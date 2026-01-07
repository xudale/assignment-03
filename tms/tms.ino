#include <WiFi.h>
#include <PubSubClient.h>
#include "sonar.h"

/* wifi network info */
const char* ssid = "Vodafone-C46752547";
const char* password = "hshPHA97mAxKn6Nc";

/* MQTT server address */
const char* mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT topic */
const char* topic = "esiot/waterDepth";

/* Sampling frequency F (ms) */ 
const unsigned long SAMPLE_PERIOD_MS = 1000;

const float TANK_HEIGHT_CM = 100.0;

const int SONAR_TRIGGER_PIN = 26;
const int SONAR_ECHO_PIN = 25;

const int GREEN_LED_PIN = 2;  
const int RED_LED_PIN = 4;

WiFiClient espClient;
PubSubClient mqtt(espClient);

enum class State { INIT, WIFI_CONNECTING, MQTT_CONNECTING, FUNCTIONING, NET_ERROR };
State state = State::INIT;

unsigned long lastSampleMs = 0;
unsigned long lastConnectAttemptMs = 0;

Sonar* pSonar = new Sonar(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN, 30000);

static void setLeds(bool ok) {
  digitalWrite(GREEN_LED_PIN, ok ? HIGH : LOW);
  digitalWrite(RED_LED_PIN,   ok ? LOW  : HIGH);
}

static float distanceToDepthInCm(float distanceCm) {
  if (distanceCm < 0) return -1.0f;
  float depth = TANK_HEIGHT_CM - distanceCm;
  if (depth < 0) {
    depth = 0;
  } 
  if (depth > TANK_HEIGHT_CM) {
    depth = TANK_HEIGHT_CM;  
  } 
  return depth;
}

static bool mqttEnsureConnected() {
  if (mqtt.connected()) return true;
  String clientId = String("esiot-2025-client-water-depth")+String(random(0xffff), HEX);
  return mqtt.connect(clientId.c_str());
}

static void publishWaterDepth(float waterDepth) {
  // {"timestamp":123456,"waterDepth":78.9}
  unsigned long ts = millis();
  char payload[128];
  snprintf(payload, sizeof(payload), "{\"timestamp\":%lu,\"waterDepth\":%.2f}", ts, waterDepth);

  bool ok = mqtt.publish(topic, payload);
  if (!ok) {
    state = State::NET_ERROR;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  setLeds(false);

  pinMode(SONAR_TRIGGER_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);

  WiFi.mode(WIFI_STA);
  mqtt.setServer(mqtt_server, 1883);

  state = State::WIFI_CONNECTING;
}

void loop() {
  const unsigned long now = millis();

  switch (state) {
    case State::WIFI_CONNECTING: {
      setLeds(false);
      if (WiFi.status() != WL_CONNECTED) {
        if (now - lastConnectAttemptMs > 3000) {
          lastConnectAttemptMs = now;
          Serial.println("TMS: connecting WiFi...");
          WiFi.begin(ssid, password);
        }
        delay(50);
        return;
      }
      Serial.print("TMS: WiFi connected, IP=");
      Serial.println(WiFi.localIP());
      state = State::MQTT_CONNECTING;
      return;
    }

    case State::MQTT_CONNECTING: {
      setLeds(false);
      if (WiFi.status() != WL_CONNECTED) {
        state = State::WIFI_CONNECTING;
        return;
      }
      if (now - lastConnectAttemptMs > 2000) {
        lastConnectAttemptMs = now;
        Serial.println("TMS: connecting MQTT...");
        if (mqttEnsureConnected()) {
          Serial.println("TMS: MQTT connected");
          state = State::FUNCTIONING;
          lastSampleMs = 0;
        }
      }
      mqtt.loop();
      delay(20);
      return;
    }

    case State::FUNCTIONING: {
      if (WiFi.status() != WL_CONNECTED || !mqtt.connected()) {
        state = State::NET_ERROR;
        return;
      }
      setLeds(true);
      mqtt.loop();

      if (now - lastSampleMs >= SAMPLE_PERIOD_MS) {
        lastSampleMs = now;
        float dist = pSonar->getDistance();
        float waterDepth = distanceToDepthInCm(dist);

        if (waterDepth < 0) {
          Serial.println("TMS: sonar read error");
          publishWaterDepth(-1.0f);
        } else {
          Serial.print("TMS: waterDepth=");
          Serial.println(waterDepth);
          publishWaterDepth(waterDepth);
        }
      }
      return;
    }

    case State::NET_ERROR: {
      setLeds(false);
      if (WiFi.status() != WL_CONNECTED) {
        state = State::WIFI_CONNECTING;
        return;
      }
      // Try reconnect MQTT
      if (!mqtt.connected()) {
        state = State::MQTT_CONNECTING;
        return;
      }
      // If we are here with MQTT connected, recover
      state = State::FUNCTIONING;
      return;
    }

    case State::INIT:
      state = State::WIFI_CONNECTING;
      return;
  }
}
