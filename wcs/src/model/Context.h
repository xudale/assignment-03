#ifndef __CONTEXT__
#define __CONTEXT__

#include "Arduino.h"
#include "tasks/state.h"
class Context {

public:
  Context();

  void setModeState(MODE_STATE newState);
  MODE_STATE getModeState();

  void setPotState(POT_STATE newState);
  POT_STATE getPotState();

  void setPercentage(int percentage);
  int getPercentage();

private:
  MODE_STATE modeState; // UNCONNECTED, AUTOMATIC, MANUAL
  POT_STATE potState; // IDLE, ACTIVE
  int percentage;
};

#endif
