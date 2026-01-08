#ifndef __TIMING_TASK__
#define __TIMING_TASK__

#include "kernel/Task.h"

class TimingTask: public Task {

public:
  virtual void tick() = 0;

protected:  
  long elapsedTimeInState() {
      return millis() - stateTimestamp;
  }
  long stateTimestamp;
};

#endif