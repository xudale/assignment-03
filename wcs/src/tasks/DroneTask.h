#ifndef __DRONE_TASK__
#define __DRONE_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include <Arduino.h>
#include "TimingTask.h"

class DroneTask: public TimingTask {

public:
  DroneTask(Context* pContext); 
  void tick();

private:  
  void setState(DRONE_STATE state);
  Context* pContext;
};

#endif