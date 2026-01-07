#include <WiFi.h>
#include <PubSubClient.h>

// --------- USER CONFIG ----------
const char* WIFI_SSID = "Vodafone-C46752547";
const char* WIFI_PASS = "hshPHA97mAxKn6Nc";

// If broker is your PC running Mosquitto or your Spring Boot app is NOT an MQTT broker.
// Use a real broker address (Mosquitto on PC recommended).
const char* MQTT_HOST = "broker.mqtt-dashboard.com";
const int   MQTT_PORT = 1883;

const char* TOPIC_LEVEL = "esiot/tank/level";

// Sampling frequency F (ms)
const unsigned long SAMPLE_PERIOD_MS = 1000;

// Thresholds are handled by CUS; TMS only measures.
// Tank geometry (example): tank height in cm, used to compute level from distance
const float TANK_HEIGHT_CM = 100.0;

// Pins (change to your wiring)
const int PIN_TRIG = 26;
const int PIN_ECHO = 25;

const int PIN_LED_GREEN = 2;  
const int PIN_LED_RED   = 4;
// --------------------------------

WiFiClient espClient;
PubSubClient mqtt(espClient);

enum class State { BOOT, WIFI_CONNECTING, MQTT_CONNECTING, OPERATIONAL, NET_ERROR };
State state = State::BOOT;

unsigned long lastSampleMs = 0;
unsigned long lastConnectAttemptMs = 0;

static void setLedsOk(bool ok) {
  digitalWrite(PIN_LED_GREEN, ok ? HIGH : LOW);
  digitalWrite(PIN_LED_RED,   ok ? LOW  : HIGH);
}

static float readDistanceCm() {
  // HC-SR04 style
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 30000UL); // 30ms timeout ~ 5m
  if (duration == 0) return -1.0f;

  // speed of sound ~343m/s => 29.1 us/cm round-trip => cm = duration/58
  float dist = duration / 58.0f;
  return dist;
}

static float distanceToLevelCm(float distanceCm) {
  // If sensor mounted on top: level = tankHeight - distance
  if (distanceCm < 0) return -1.0f;
  float level = TANK_HEIGHT_CM - distanceCm;
  if (level < 0) level = 0;
  if (level > TANK_HEIGHT_CM) level = TANK_HEIGHT_CM;
  return level;
}

static bool mqttEnsureConnected() {
  if (mqtt.connected()) return true;

  String clientId = "tms-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  // No auth in this prototype
  return mqtt.connect(clientId.c_str());
}

static void publishLevel(float levelCm) {
  // Minimal JSON
  // {"ts":123456,"level_cm":42.3}
  unsigned long ts = millis();
  char payload[128];
  snprintf(payload, sizeof(payload), "{\"ts\":%lu,\"level_cm\":%.2f}", ts, levelCm);

  bool ok = mqtt.publish(TOPIC_LEVEL, payload);
  if (!ok) {
    state = State::NET_ERROR;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  setLedsOk(false);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  WiFi.mode(WIFI_STA);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);

  state = State::WIFI_CONNECTING;
}

void loop() {
  const unsigned long now = millis();

  switch (state) {
    case State::WIFI_CONNECTING: {
      setLedsOk(false);
      if (WiFi.status() != WL_CONNECTED) {
        if (now - lastConnectAttemptMs > 3000) {
          lastConnectAttemptMs = now;
          Serial.println("TMS: connecting WiFi...");
          WiFi.begin(WIFI_SSID, WIFI_PASS);
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
      setLedsOk(false);
      if (WiFi.status() != WL_CONNECTED) {
        state = State::WIFI_CONNECTING;
        return;
      }
      if (now - lastConnectAttemptMs > 2000) {
        lastConnectAttemptMs = now;
        Serial.println("TMS: connecting MQTT...");
        if (mqttEnsureConnected()) {
          Serial.println("TMS: MQTT connected");
          state = State::OPERATIONAL;
          lastSampleMs = 0;
        }
      }
      mqtt.loop();
      delay(20);
      return;
    }

    case State::OPERATIONAL: {
      if (WiFi.status() != WL_CONNECTED || !mqtt.connected()) {
        state = State::NET_ERROR;
        return;
      }
      setLedsOk(true);
      mqtt.loop();

      if (now - lastSampleMs >= SAMPLE_PERIOD_MS) {
        lastSampleMs = now;
        float dist = readDistanceCm();
        float level = distanceToLevelCm(dist);

        if (level < 0) {
          Serial.println("TMS: sonar read error");
          // still publish? optional. Here publish -1 as invalid.
          publishLevel(-1.0f);
        } else {
          Serial.print("TMS: level_cm=");
          Serial.println(level);
          publishLevel(level);
        }
      }
      return;
    }

    case State::NET_ERROR: {
      setLedsOk(false);
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
      state = State::OPERATIONAL;
      return;
    }

    default:
      state = State::WIFI_CONNECTING;
      return;
  }
}
