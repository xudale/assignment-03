package xu.pengyue.controller;

import org.springframework.beans.factory.annotation.Autowired;
import xu.pengyue.dto.TankStatusDto;
import xu.pengyue.service.ArduinoBridge;
import xu.pengyue.service.PolicyService;
import xu.pengyue.service.TankService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

@RestController
@RequestMapping("/api")
@CrossOrigin(origins = "*")
public class TankController {

    @Autowired
    private TankService tankService;

    @Autowired
    private ArduinoBridge arduinoBridge;

    @GetMapping("/status")
    public TankStatusDto getStatus() {
        return tankService.getStatus();
    }

    @PostMapping("/mode")
    public ResponseEntity<Void> setMode(@RequestBody Map<String, String> body) {
        String mode = body.get("mode");
        if ("MANUAL".equalsIgnoreCase(mode)) {
            tankService.setMode(TankService.Mode.MANUAL);
            arduinoBridge.sendMode("MANUAL");
            return ResponseEntity.ok().build();
        }
        if ("AUTOMATIC".equalsIgnoreCase(mode)) {
            tankService.setMode(TankService.Mode.AUTOMATIC);
            arduinoBridge.sendMode("AUTOMATIC");
            return ResponseEntity.ok().build();
        }
        return ResponseEntity.badRequest().build();
    }

    @PostMapping("/valve")
    public ResponseEntity<Void> setValve(@RequestBody Map<String, Integer> body) {
        Integer percentage = body.get("percentage");
        if (percentage < 0 || percentage > 100) {
            return ResponseEntity.badRequest().build();
        }
        tankService.updateValvePercentage(percentage);
        arduinoBridge.sendPercentage(percentage);
        return ResponseEntity.ok().build();
    }
}
