#include "Context.h"

Context::Context(){
  droneState = DRONE_STATE::REST;
  hangarDoorState = HANGAR_DOOR_STATE::CLOSED;
  alarmState = ALARM_STATE::NORMAL;
  modeState = MODE_STATE::UNCONNECTED;
  distance = 0;
}

void Context::setDroneState(DRONE_STATE newState) {
  droneState = newState;
}
DRONE_STATE Context::getDroneState() {
  return droneState;
}

void Context::setModeState(MODE_STATE newState) {
  modeState = newState;
}
MODE_STATE Context::getModeState() {
  return modeState;
}

void Context::setHangarDoorState(HANGAR_DOOR_STATE newState) {
  hangarDoorState = newState;
}
HANGAR_DOOR_STATE Context::getHangarDoorState() {
  return hangarDoorState;
}

void Context::setAlarmState(ALARM_STATE newState) {
  alarmState = newState;
}
ALARM_STATE Context::getAlarmState(){
  return alarmState;
}

void Context::setDistance(int distance) {
  this->distance = distance;
}
int Context::getDistance() {
  return distance;
}

