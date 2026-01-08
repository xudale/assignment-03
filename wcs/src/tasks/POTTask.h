#ifndef __POT_TASK__
#define __POT_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Pot.h"
#include "devices/ServoMotor.h"
#include <Arduino.h>

class POTTask: public Task {

public:
  POTTask(Potentiometer* pPOT,ServoMotor *pMotor, Context* pContext); 
  void tick();

private:  
  Context* pContext;
  Potentiometer* pPOT;
  ServoMotor* pMotor;
};

#endif  