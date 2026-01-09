package xu.pengyue.serial;

import jssc.*;

public class SerialPorts {

    public String getUSBPortName() {
        String[] portNames = SerialPortList.getPortNames();
        for (String portName : portNames) {
            if (portName.startsWith("/dev/tty.usb")) {
                return portName;
            }
        }
        return null;
    }

}
