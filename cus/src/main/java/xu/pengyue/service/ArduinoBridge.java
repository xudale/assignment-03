package xu.pengyue.service;

import jakarta.annotation.PostConstruct;
import jakarta.annotation.PreDestroy;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;
import xu.pengyue.config.AppConfig;
import xu.pengyue.serial.SerialCommChannel;
import xu.pengyue.serial.SerialPorts;

@Service
public class ArduinoBridge {
    private static final Logger logger = LoggerFactory.getLogger(ArduinoBridge.class);

    private final AppConfig cfg;
    private final TankService tankService;

    private SerialCommChannel channel;
    private volatile boolean running = true;

    public ArduinoBridge(AppConfig cfg, TankService tankService) {
        this.cfg = cfg;
        this.tankService = tankService;
    }

    @PostConstruct
    public void start() {
        try {
            String usbPortName = new SerialPorts().getUSBPortName();
            if (usbPortName == null) {
                logger.warn("No USB serial port detected. WCS bridge disabled.");
                return;
            }
            channel = new SerialCommChannel(usbPortName, cfg.getSerial().getBaud());
            logger.info("Connected to WCS on {} at {} baud", usbPortName, cfg.getSerial().getBaud());

            Thread t = new Thread(this::receiveLoop, "arduino-receiver");
            t.setDaemon(true);
            t.start();
        } catch (Exception ex) {
            logger.error("Failed to initialize Arduino bridge", ex);
        }
    }

    private void receiveLoop() {
        while (running && channel != null) {
            try {
                String msg = channel.receiveMsg();
                handleArduinoMessage(msg.trim());
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            } catch (Exception e) {
                logger.warn("Error while receiving from Arduino", e);
            }
        }
    }

    private void handleArduinoMessage(String msg) {
        if (msg.startsWith("MODE_STATE::")) {
            String mode = msg.substring("MODE_STATE::".length()).trim();
            if ("AUTOMATIC".equalsIgnoreCase(mode)) {
                tankService.updateFromWcsMode(TankService.Mode.AUTOMATIC);
            } else if ("MANUAL".equalsIgnoreCase(mode)) {
                tankService.updateFromWcsMode(TankService.Mode.MANUAL);
            } else if ("UNCONNECTED".equalsIgnoreCase(mode)) {
                tankService.updateFromWcsMode(TankService.Mode.UNCONNECTED);
            }
            return;
        }

        if (msg.startsWith("Sync:Percentage:")) {
            String value = msg.substring("Sync:Percentage:".length()).trim();
            try {
                int percentage = Integer.parseInt(value);
                tankService.updateValvePercentage(percentage);
            } catch (NumberFormatException ex) {
                logger.debug("Invalid percentage from WCS: {}", msg);
            }
            return;
        }

        logger.debug("WCS: {}", msg);
    }

    public void sendMode(String mode) {
        if (channel == null) {
            return;
        }
        channel.sendMsg("MODE:" + mode);
    }

    public void sendPercentage(int percentage) {
        if (channel == null) {
            return;
        }
        channel.sendMsg("PERCENTAGE:" + percentage);
    }

    @PreDestroy
    public void shutdown() {
        running = false;
        if (channel != null) {
            channel.close();
        }
    }
}
