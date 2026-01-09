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
    private Mode mode = Mode.AUTOMATIC;
    private Mode requestedMode = Mode.AUTOMATIC;
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
        this.valvePercentage = Math.max(0, Math.min(100, valvePercentage));
    }

    public synchronized void setMode(Mode mode) {
        this.mode = mode;
    }

    public synchronized void setRequestedMode(Mode mode) {
        this.requestedMode = mode;
        if (this.mode != Mode.UNCONNECTED) {
            this.mode = mode;
        }
    }

    public synchronized Mode getRequestedMode() {
        return requestedMode;
    }

    public synchronized Mode getMode() {
        return mode;
    }

    public synchronized void markUnconnected() {
        this.mode = Mode.UNCONNECTED;
    }

    public synchronized void recoverFromUnconnected() {
        if (mode == Mode.UNCONNECTED) {
            mode = requestedMode;
        }
    }

    public synchronized void updateFromWcsMode(Mode mode) {
        if (mode == Mode.UNCONNECTED) {
            this.mode = Mode.UNCONNECTED;
        } else {
            this.requestedMode = mode;
            if (this.mode != Mode.UNCONNECTED) {
                this.mode = mode;
            }
        }
    }

    public synchronized TankStatusDto getStatus() {
        return new TankStatusDto(
                waterDepth,
                valvePercentage,
                mode.name(),
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
