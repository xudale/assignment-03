package xu.pengyue.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class TankStatusDto {
    private double waterDepth;
    private int valvePercentage;
    private String mode;
    private String systemState;
    private long lastUpdateMs;
    private List<WaterLevelSampleDto> history;
}
