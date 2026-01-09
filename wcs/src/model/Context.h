#ifndef __CONTEXT__
#define __CONTEXT__

#include "Arduino.h"
#include "tasks/state.h"
class Context {

public:
  Context();

  void setHangarDoorState(HANGAR_DOOR_STATE newState);
  HANGAR_DOOR_STATE getHangarDoorState();

  void setModeState(MODE_STATE newState);
  MODE_STATE getModeState();

  void setPotState(POT_STATE newState);
  POT_STATE getPotState();

  void setDistance(int distance);
  int getDistance();

private:
  HANGAR_DOOR_STATE hangarDoorState;  // CLOSED, OPEN
  MODE_STATE modeState; // UNCONNECTED, AUTOMATIC, MANUAL
  POT_STATE potState; // IDLE, ACTIVE
  int distance;
};

#endif
