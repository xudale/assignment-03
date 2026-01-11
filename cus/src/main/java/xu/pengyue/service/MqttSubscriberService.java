package xu.pengyue.service;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import jakarta.annotation.PostConstruct;
import jakarta.annotation.PreDestroy;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;
import xu.pengyue.config.AppConfig;

@Service
public class MqttSubscriberService {

    private static final Logger logger = LoggerFactory.getLogger(MqttSubscriberService.class);

    private final AppConfig config;
    private final PolicyService policyService;
    private final ObjectMapper objectMapper;

    private MqttClient client;

    public MqttSubscriberService(AppConfig config, PolicyService policyService, ObjectMapper objectMapper) {
        this.config = config;
        this.policyService = policyService;
        this.objectMapper = objectMapper;
    }

    @PostConstruct
    public void start() {
        try {
            String clientId = "cus-" + System.currentTimeMillis();
            client = new MqttClient(config.getMqtt().getHost(), clientId);
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    logger.warn("MQTT connection lost", cause);
                }

                @Override
                public void messageArrived(String topic, MqttMessage message) {
                    handleMessage(message);
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                }
            });

            client.connect();
            client.subscribe(config.getMqtt().getTopicLevel(), 1);
            logger.info("Subscribed to {}", config.getMqtt().getTopicLevel());
        } catch (MqttException e) {
            logger.error("Failed to start MQTT subscriber", e);
        }
    }

    private void handleMessage(MqttMessage message) {
        try {
            JsonNode node = objectMapper.readTree(message.getPayload());
            double waterDepth = node.path("waterDepth").asDouble(-1.0);
            long timestamp = node.path("timestamp").asLong(System.currentTimeMillis());
            policyService.onWaterDepth(waterDepth, timestamp);
            System.out.println(waterDepth);
        } catch (Exception ex) {
            logger.warn("Failed to parse MQTT payload: {}", new String(message.getPayload()), ex);
        }
    }

    @PreDestroy
    public void shutdown() {
        if (client != null) {
            try {
                client.disconnect();
                client.close();
            } catch (MqttException e) {
                logger.warn("Failed to close MQTT client", e);
            }
        }
    }
}
