package xu.pengyue.service;

import jakarta.annotation.PostConstruct;
import jakarta.annotation.PreDestroy;
import xu.pengyue.config.AppConfig;
import xu.pengyue.serial.SerialCommChannel;
import xu.pengyue.serial.SerialPorts;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class ArduinoBridge {
    private final AppConfig cfg;
    private final SerialCommChannel channel;

    @Autowired
    private  HangarService hangarService;   // your service that holds state

    private volatile boolean running = true;

    public ArduinoBridge(AppConfig cfg) throws Exception {
        this.cfg = cfg;
        String usbPortName = new SerialPorts().getUSBPortName();
        System.out.println(usbPortName);
        this.channel  = new SerialCommChannel(usbPortName,this.cfg.serial.baud);
        System.out.println(this.cfg.serial.baud);
    }

    @PostConstruct
    public void start() {
        Thread t = new Thread(this::receiveLoop, "arduino-receiver");
        t.setDaemon(true);
        t.start();
    }

    private void receiveLoop() {
        while (running) {
            try {
                String msg = channel.receiveMsg();  // blocking
                handleArduinoMessage(msg.trim());
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void handleArduinoMessage(String msg) {
//        System.out.println("From Arduino: " + msg);
        hangarService.updateFromArduino(msg);
    }

    public void sendCommand(String cmd) {
        channel.sendMsg(cmd);
    }

    @PreDestroy
    public void shutdown() {
        running = false;
        channel.close();
    }
}
