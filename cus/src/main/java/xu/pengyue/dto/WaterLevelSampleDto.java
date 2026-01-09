package xu.pengyue.dto;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class WaterLevelSampleDto {
    private long timestamp;
    private double waterDepth;
}
