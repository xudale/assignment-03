#ifndef __LED1_TASK__
#define __LED1_TASK__

#include "tasks/LEDTask.h"

#include <Arduino.h>

class LED1Task: public LEDTask {

public:
  LED1Task(Led* pLed, Context* pContext); 
  void tick() override;
};


#endif