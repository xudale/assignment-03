package xu.pengyue.controller;


import xu.pengyue.dto.HangarStatusDto;
import xu.pengyue.service.HangarService;
import xu.pengyue.service.ArduinoBridge;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.Map;

@RestController
@RequestMapping("/api")
@CrossOrigin(origins = "*")
public class HangarController {

    @Autowired
    private HangarService hangarService;

    @Autowired
    private ArduinoBridge arduinoBridge;

    @PostMapping("/cmd")
    public ResponseEntity<Void> sendCommand(@RequestBody Map<String, String> body) {
        String cmd = body.get("cmd");
        if (cmd == null) {
            return ResponseEntity.badRequest().build();
        }

        // send to Arduino over serial
        arduinoBridge.sendCommand(cmd);


        return ResponseEntity.ok().build();
    }

    @GetMapping("/status")
    public HangarStatusDto getStatus() {
        return hangarService.getStatus();
    }
}
