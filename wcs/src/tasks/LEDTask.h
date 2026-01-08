#ifndef __LED_TASK__
#define __LED_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Led.h"
#include <Arduino.h>

class LEDTask: public Task {

public:
  LEDTask(Led* pLed, Context* pContext): pLed(pLed), pContext(pContext){}; 
  virtual void tick() = 0;

protected:  
  void setState(LED_STATE state) {
      this->state = state;
      justEntered = true;
  }
  
  bool checkAndSetJustEntered() {
      if (justEntered) {
          justEntered = false; 
          return true;
      }
      return false;
  }
  Led* pLed;
  Context* pContext;
  LED_STATE state;
private:
  bool justEntered;
};

#endif