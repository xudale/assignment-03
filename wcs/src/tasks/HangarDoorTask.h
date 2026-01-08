#ifndef __HANGAR_DOOR_TASK__
#define __HANGAR_DOOR_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include <Arduino.h>
#include "devices/Sonar.h"
#include "devices/ServoMotor.h"
#include "devices/Pir.h"
#include "TimingTask.h"

class HangarDoorTask: public TimingTask {

public:
  HangarDoorTask(Pir* pPir ,Sonar* pSonar, ServoMotor *pMotor,Context* pContext); 
  void tick();

private:  
  void setState(HANGAR_DOOR_STATE state);
  HANGAR_DOOR_STATE state;
  Sonar* pSonar;
  ServoMotor* pMotor;
  Pir* pPir;
  Context* pContext;
  bool waitingT1;
  bool waitingT2;
};

#endif