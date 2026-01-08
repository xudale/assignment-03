#ifndef __LED3_TASK__
#define __LED3_TASK__

#include "tasks/LEDTask.h"

#include <Arduino.h>

class LED3Task: public LEDTask {

public:
  LED3Task(Led* pLed, Context* pContext); 
  void tick() override;
};


#endif