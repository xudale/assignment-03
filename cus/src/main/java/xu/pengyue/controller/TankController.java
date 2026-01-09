package xu.pengyue.controller;

import xu.pengyue.dto.TankStatusDto;
import xu.pengyue.service.PolicyService;
import xu.pengyue.service.TankService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

@RestController
@RequestMapping("/api")
@CrossOrigin(origins = "*")
public class TankController {

    private final TankService tankService;
    private final PolicyService policyService;

    public TankController(TankService tankService, PolicyService policyService) {
        this.tankService = tankService;
        this.policyService = policyService;
    }

    @GetMapping("/status")
    public TankStatusDto getStatus() {
        return tankService.getStatus();
    }

    @PostMapping("/mode")
    public ResponseEntity<Void> setMode(@RequestBody Map<String, String> body) {
        String mode = body.get("mode");
        if (mode == null) {
            return ResponseEntity.badRequest().build();
        }
        if ("MANUAL".equalsIgnoreCase(mode)) {
            policyService.requestMode(TankService.Mode.MANUAL);
            return ResponseEntity.ok().build();
        }
        if ("AUTOMATIC".equalsIgnoreCase(mode)) {
            policyService.requestMode(TankService.Mode.AUTOMATIC);
            return ResponseEntity.ok().build();
        }
        return ResponseEntity.badRequest().build();
    }

    @PostMapping("/valve")
    public ResponseEntity<Void> setValve(@RequestBody Map<String, Integer> body) {
        Integer percentage = body.get("percentage");
        if (percentage == null) {
            return ResponseEntity.badRequest().build();
        }
        if (percentage < 0 || percentage > 100) {
            return ResponseEntity.badRequest().build();
        }
        policyService.requestManualPercentage(percentage);
        return ResponseEntity.ok().build();
    }
}
