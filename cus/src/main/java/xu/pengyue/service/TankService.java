package xu.pengyue.service;

import xu.pengyue.config.AppConfig;
import xu.pengyue.dto.TankStatusDto;
import xu.pengyue.dto.WaterLevelSampleDto;
import org.springframework.stereotype.Service;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;

@Service
public class TankService {

    public enum Mode {
        AUTOMATIC,
        MANUAL,
        UNCONNECTED
    }

    private final int historySize;
    private final Deque<WaterLevelSampleDto> history;
    private double waterDepth;
    private int valvePercentage;
    private Mode mode = Mode.UNCONNECTED;
    private long lastUpdateMs;

    public TankService(AppConfig config) {
        this.historySize = config.getHistory().getN();
        this.history = new ArrayDeque<>(historySize);
    }

    public synchronized void recordWaterDepth(double waterDepth, long timestamp) {
        this.waterDepth = waterDepth;
        this.lastUpdateMs = System.currentTimeMillis();
        history.addLast(new WaterLevelSampleDto(timestamp, waterDepth));
        while (history.size() > historySize) {
            history.removeFirst();
        }
    }

    public synchronized void updateValvePercentage(int valvePercentage) {
        this.valvePercentage = valvePercentage;
    }

    public synchronized void setMode(Mode mode) {
        this.mode = mode;
    }

    public synchronized Mode getMode() {
        return mode;
    }

    public synchronized TankStatusDto getStatus() {
        return new TankStatusDto(
                waterDepth,
                valvePercentage,
                mode.name(),
                lastUpdateMs,
                new ArrayList<>(history)
        );
    }

    public synchronized long getLastUpdateMs() {
        return lastUpdateMs;
    }

    public synchronized List<WaterLevelSampleDto> getHistorySnapshot() {
        return new ArrayList<>(history);
    }
}
