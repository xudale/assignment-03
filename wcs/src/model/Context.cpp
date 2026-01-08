#include "Context.h"

Context::Context(){
  droneState = DRONE_STATE::REST;
  hangarDoorState = HANGAR_DOOR_STATE::CLOSED;
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

void Context::setPotState(POT_STATE newState) {
  potState = newState;
}
POT_STATE Context::getPotState() {
  return potState;
}

void Context::setHangarDoorState(HANGAR_DOOR_STATE newState) {
  hangarDoorState = newState;
}
HANGAR_DOOR_STATE Context::getHangarDoorState() {
  return hangarDoorState;
}

void Context::setDistance(int distance) {
  this->distance = distance;
}
int Context::getDistance() {
  return distance;
}

