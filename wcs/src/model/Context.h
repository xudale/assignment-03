#ifndef __CONTEXT__
#define __CONTEXT__

#include "Arduino.h"
#include "tasks/state.h"
class Context {

public:
  Context();

  void setDroneState(DRONE_STATE newState);
  DRONE_STATE getDroneState();

  void setHangarDoorState(HANGAR_DOOR_STATE newState);
  HANGAR_DOOR_STATE getHangarDoorState();

  void setAlarmState(ALARM_STATE newState);
  ALARM_STATE getAlarmState();

  void setModeState(MODE_STATE newState);
  MODE_STATE getModeState();

  void setDistance(int distance);
  int getDistance();
  
private:
  DRONE_STATE droneState;   // REST, TAKING OFF, OUT, LANDING, OUT_WITH_ALARM
  HANGAR_DOOR_STATE hangarDoorState;  // CLOSED, OPEN
  ALARM_STATE alarmState;  // NORMAL, PRE-ALARM, ALARM
  MODE_STATE modeState; // UNCONNECTED, AUTOMATIC, MANUAL
  int distance;
};

#endif
