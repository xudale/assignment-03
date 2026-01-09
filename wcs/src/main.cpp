#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"

#include "tasks/HangarDoorTask.h"
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

  Task* pModeTask = new ModeTask(pHWPlatform->getButton(), pContext);
  pModeTask->init(1000);

  Task* pPOTTask = new POTTask(pHWPlatform->getPot(), pHWPlatform->getMotor(), pContext);
  pPOTTask->init(500);

  sched.addTask(pHangarDoorTask);
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
        pContext->setPotState(POT_STATE::IDLE);
        Logger.log("dale Set mode to AUTO");
      } else if(mode == "MANUAL") {
        pContext->setModeState(MODE_STATE::MANUAL);
        pContext->setPotState(POT_STATE::ACTIVE);
        Logger.log("dale Set mode to MANUAL");
      }
    } else if(content.startsWith("PERCENTAGE:")) {
      String percentageStr = content.substring(11);
      int percentage = percentageStr.toInt();
      Logger.log("dale Set POT percentage to " + String(percentage));
      if (percentage < 0) {
        pContext->setModeState(MODE_STATE::UNCONNECTED);
        pContext->setPotState(POT_STATE::IDLE);
      } else {
        pContext->setPotState(POT_STATE::IDLE);
        int angle = (int)(percentage * 90); // Scale to 0-90 degrees
        pHWPlatform->getMotor()->on();
        pHWPlatform->getMotor()->setPosition(angle);
      } 
      if (percentage > 0 && pContext->getModeState() == MODE_STATE::UNCONNECTED) {
        Logger.log("dale in error");
      }
    }
    delete msg;
  }
}

void sendMessage() {
    static int count = 0;
    lcd.setCursor(4, 1); 
    if (count++ >= 10) {
      

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