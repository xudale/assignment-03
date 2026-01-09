package xu.pengyue.service;

import org.eclipse.paho.client.mqttv3.*;

public class SubscriberService {

    public static void main(String[] args) throws Exception {
        String broker = "tcp://broker.mqtt-dashboard.com";
        String clientId = "esiot-2025-" + System.currentTimeMillis();
        String topic = "esiot/waterDepth";

        MqttClient client = new MqttClient(broker, clientId);

        // Callback to handle messages and connection events
        client.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {
                System.out.println("Connection lost! " + cause.getMessage());
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) {
                System.out.println("Received message on topic " + topic + ": "
                        + new String(message.getPayload()));
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
                // Not used for subscribers
            }
        });

        client.connect();


        /*
         * Subscribing - second param is the QoS (from broker to receiver)
         *
         * - 0: at most once (minimum)
         * - 1: at least once
         * - 2: exactly once (maximum)
         */
        client.subscribe(topic, 1);

        System.out.println("Subscribed to topic: " + topic);

        // Keep the program running to listen for messages
        Thread.sleep(60000);
        client.disconnect();
        client.close();
    }
}

