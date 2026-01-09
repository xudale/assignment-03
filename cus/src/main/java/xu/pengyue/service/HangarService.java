package xu.pengyue.service;

import xu.pengyue.dto.HangarStatusDto;
import org.springframework.stereotype.Service;

@Service
public class HangarService {

    private String droneState;   // REST, TAKING OFF, OUT, LANDING
    private String hangarDoorState;  // CLOSED, OPEN
    private String alarmState;  // NORMAL, PRE-ALARM, ALARM
    private int distance;
    private String debug;
    private String lastDebug;

    public synchronized HangarStatusDto getStatus() {
        String copy = debug;
        debug = null;
        return new HangarStatusDto(droneState, hangarDoorState, alarmState, distance, copy);
    }

    public synchronized void updateFromArduino(String line) {
        if (line.startsWith("DRONE")) {
            droneState = line.substring("DRONE ".length());
        } else if (line.startsWith("HANGAR_DOOR")) {
            hangarDoorState = line.substring("HANGAR_DOOR ".length());
        } else if (line.startsWith("ALARM")) {
            alarmState = line.substring("ALARM ".length());
        } else if (line.startsWith("DISTANCE")) {
            distance = Integer.parseInt(line.substring("DISTANCE ".length()));
        } else {
            if (lastDebug != null && lastDebug.equals(line)) {
                debug = null;
            } else {
                debug = line;
                lastDebug = line;
            }
            System.out.println("From Arduino: " + line);
        }
    }

}

