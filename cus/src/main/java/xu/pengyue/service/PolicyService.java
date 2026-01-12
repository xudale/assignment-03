package xu.pengyue.service;

import org.springframework.beans.factory.annotation.Autowired;
import xu.pengyue.config.AppConfig;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

@Service
public class PolicyService {
    @Autowired
    private AppConfig config;
    @Autowired
    private TankService tankService;
    @Autowired
    private ArduinoBridge arduinoBridge;

    private Long overL1SinceMs;
    private int lastCommandedPercentage = -1;

    public void resetLastCommandedPercentageForAutomaticMode() {
        lastCommandedPercentage = -99;
    }
    /* called by mqtt */
    public void onWaterDepth(double waterDepth, long timestamp) {
        tankService.recordWaterDepth(waterDepth, timestamp);
        if (tankService.getMode() != TankService.Mode.AUTOMATIC) {
            resetLastCommandedPercentageForAutomaticMode();
        }
        if (tankService.getMode() == TankService.Mode.UNCONNECTED) {
            tankService.setMode(TankService.Mode.AUTOMATIC);
            arduinoBridge.sendMode("RESET");
        } else if (tankService.getMode() == TankService.Mode.AUTOMATIC) {
            int targetPercentage = computeAutomaticPercentage(waterDepth);
            sendValvePercentage(targetPercentage);
        }
    }

    @Scheduled(fixedDelay = 1000)
    public void checkConnectivity() {
        long lastUpdateMs = tankService.getLastUpdateMs();
        if (lastUpdateMs > 0) {
            long now = System.currentTimeMillis();
            if (now - lastUpdateMs > config.getPolicy().getT2Ms()) {
                if (tankService.getMode() != TankService.Mode.UNCONNECTED) {
                    tankService.setMode(TankService.Mode.UNCONNECTED);
                    arduinoBridge.sendPercentage(-1);
                }
            }
        }
    }

    private int computeAutomaticPercentage(double waterDepth) {
        double l1 = config.getPolicy().getL1();
        double l2 = config.getPolicy().getL2();
        long now = System.currentTimeMillis();

        if (waterDepth >= l2) {
            overL1SinceMs = null;
            return 100;
        }

        if (waterDepth > l1) {
            if (overL1SinceMs == null) {
                overL1SinceMs = now;
            }
            if (now - overL1SinceMs >= config.getPolicy().getT1Ms()) {
                return 50;
            }
            return 0;
        }

        overL1SinceMs = null;
        return 0;
    }

    private void sendValvePercentage(int percentage) {
        tankService.updateValvePercentage(percentage);
        if (percentage != lastCommandedPercentage) {
            lastCommandedPercentage = percentage;
            arduinoBridge.sendPercentage(percentage);
        }
    }
}
