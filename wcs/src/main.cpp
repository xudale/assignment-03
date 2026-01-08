#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"

#include "tasks/HangarDoorTask.h"
#include "tasks/DroneTask.h"
#include "tasks/ModeTask.h"
#include "tasks/POTTask.h"
#include <LiquidCrystal_I2C.h> 

// #define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); 
void setup() {
  lcd.init();
  lcd.backlight();
  MsgService.init();
  sched.init(50);

  Logger.log(":::::: WCS ::::::");
  
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();
  

#ifndef __TESTING_HW__
  pContext = new Context();

  Task* pHangarDoorTask = new HangarDoorTask(pHWPlatform->getPir(),  pHWPlatform->getSonar(), pHWPlatform->getMotor(), pContext);
  pHangarDoorTask->init(1000);

  Task* pDroneTask = new DroneTask(pContext);
  pDroneTask->init(500);

  Task* pModeTask = new ModeTask(pHWPlatform->getButton(), pContext);
  pModeTask->init(1000);

  Task* pPOTTask = new POTTask(pHWPlatform->getPot(), pHWPlatform->getMotor(), pContext);
  pPOTTask->init(500);

  sched.addTask(pHangarDoorTask);
  sched.addTask(pDroneTask);
  sched.addTask(pModeTask);
  sched.addTask(pPOTTask);
#endif
}

void handleMessage() {
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();
    String content = msg->getContent();
    Logger.log("Received message: " + content);
    if(content.startsWith("MODE:")) {
      String mode = content.substring(5);
      if(mode == "AUTOMATIC") {
        pContext->setModeState(MODE_STATE::AUTOMATIC);
        Logger.log("dale Set mode to AUTO");
      } else if(mode == "MANUAL") {
        pContext->setModeState(MODE_STATE::MANUAL);
        pContext->setPotState(POT_STATE::ACTIVE);
        Logger.log("dale Set mode to MANUAL");
      }
    } else if(content.startsWith("ANGLE:")) {
      String angleStr = content.substring(6);
      int angle = angleStr.toInt();
      Logger.log("dale Set POT angle to " + String(angle));
      if (pContext->getModeState() == MODE_STATE::MANUAL) {
        pContext->setPotState(POT_STATE::IDLE);
      }
      // todo set potentiometer angle considering angle from the server and potentiometer
        
    }
    delete msg;
  }
}

void sendMessage() {
    static int count = 0;
    lcd.setCursor(4, 1); 
    if (count++ >= 10) {
      if (pContext ->getDroneState() == DRONE_STATE::REST) {
        Logger.log(F("DRONE REST"));
        lcd.print("DRONE INSIDE");
      } else if (pContext ->getDroneState() == DRONE_STATE::TAKING_OFF) {
        Logger.log(F("DRONE TAKING_OFF"));
        lcd.print("  TAKE OFF  ");
      } else if (pContext ->getDroneState() == DRONE_STATE::OUT) {
        Logger.log(F("DRONE OUT"));
        lcd.print(" DRONE OUT  ");
      } else if (pContext ->getDroneState() == DRONE_STATE::OUT_WITH_ALARM) {
        Logger.log(F("DRONE OUT WITH ALARM"));
        lcd.print(" DRONE OUT  ");
      } else if (pContext ->getDroneState() == DRONE_STATE::LANDING) {
        Logger.log(F("DRONE LANDING"));
        lcd.print("  LANDING   ");
      }

      if (pContext ->getHangarDoorState() == HANGAR_DOOR_STATE::CLOSED) {
        Logger.log(F("HANGAR_DOOR CLOSED"));
      } else if (pContext ->getHangarDoorState() == HANGAR_DOOR_STATE::OPEN) {
        Logger.log(F("HANGAR_DOOR OPEN"));
      }

      int distance = pContext->getDistance();
      Logger.log("DISTANCE " + String(distance));

      count = 0;
    }
}

void loop() {
    sched.schedule();
    handleMessage();
    sendMessage();
}