#include "Context.h"

Context::Context(){
  modeState = MODE_STATE::UNCONNECTED;
  percentage = 0;
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

void Context::setPercentage(int percentage) {
  this->percentage = percentage;
}
int Context::getPercentage() {
  return percentage;
}

