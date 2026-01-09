package xu.pengyue.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class HangarStatusDto {
    private String droneState;   // REST, TAKING OFF, OUT, LANDING, OUt-ALARM
    private String hangarDoorState;  // CLOSED, OPEN
    private String alarmState;  // NORMAL, PRE-ALARM, ALARM
    private int distance;
    private String debug;
}

