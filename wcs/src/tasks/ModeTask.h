#ifndef __MODE_TASK__
#define __MODE_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Button.h"
#include <Arduino.h>

class ModeTask: public Task {

public:
  ModeTask(Button* pButton, Context* pContext); 
  void tick();

private:  
  Context* pContext;
  Button* pButton;
};

#endif