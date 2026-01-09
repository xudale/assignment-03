package xu.pengyue.service;

import jakarta.annotation.PostConstruct;
import jakarta.annotation.PreDestroy;
import xu.pengyue.serial.SerialCommChannel;
import xu.pengyue.serial.SerialPorts;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class ArduinoBridge {

    private final SerialCommChannel channel;

    @Autowired
    private  HangarService hangarService;   // your service that holds state

    private volatile boolean running = true;

    public ArduinoBridge() throws Exception {
        String usbPortName = new SerialPorts().getUSBPortName();
        System.out.println(usbPortName);
        this.channel  = new SerialCommChannel(usbPortName,115200);
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
