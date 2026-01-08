#ifndef __LED2_TASK__
#define __LED2_TASK__

#include "tasks/LEDTask.h"

#include <Arduino.h>

class LED2Task: public LEDTask {

public:
  LED2Task(Led* pLed, Context* pContext); 
  void tick() override;
};




#endif