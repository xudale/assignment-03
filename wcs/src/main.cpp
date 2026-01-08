#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"

#include "tasks/AlarmTask.h"
#include "tasks/HangarDoorTask.h"
#include "tasks/DroneTask.h"
#include <LiquidCrystal_I2C.h> 

// #define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;
AlarmTask* pAlarmTask;

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

  pAlarmTask = new AlarmTask(pHWPlatform->getButton() ,pHWPlatform->getTempSensor(), pContext);
  pAlarmTask->init(1000);

  Task* pHangarDoorTask = new HangarDoorTask(pHWPlatform->getPir(),  pHWPlatform->getSonar(), pHWPlatform->getMotor(), pContext);
  pHangarDoorTask->init(1000);

  Task* pDroneTask = new DroneTask(pContext);
  pDroneTask->init(500);

  sched.addTask(pAlarmTask);
  sched.addTask(pHangarDoorTask);
  sched.addTask(pDroneTask);
#endif
}

void handleMessage() {
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();
    Logger.log("Received message: " + msg->getContent());
    if(msg->getContent() == "TAKEOFF" && 
        pContext->getAlarmState() != ALARM_STATE::ALARM &&
        pContext->getAlarmState() != ALARM_STATE::PRE_ALARM) {
        pContext->setDroneState(DRONE_STATE::TAKING_OFF);
    } else if(msg->getContent() == "LAND" && 
              pContext->getAlarmState() != ALARM_STATE::ALARM &&
              pContext->getAlarmState() != ALARM_STATE::PRE_ALARM) {
        pContext->setDroneState(DRONE_STATE::LANDING);
    } else if(msg->getContent().startsWith("FAKETEMP")) {
      int fakeTemp = msg->getContent().substring(9).toInt();
      pAlarmTask->setFakeTemperature(fakeTemp);
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
      if (pContext ->getAlarmState() == ALARM_STATE::NORMAL) {
        Logger.log(F("ALARM NORMAL"));
      } else if (pContext ->getAlarmState() == ALARM_STATE::PRE_ALARM) {
        Logger.log(F("ALARM PRE-ALARM"));
      } else if (pContext ->getAlarmState() == ALARM_STATE::ALARM) {
        Logger.log(F("ALARM ALARM"));
        lcd.setCursor(4, 1); 
        lcd.print("   ALARM    ");
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