#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include <Arduino.h>
#include "devices/TempSensorLM35.h"
#include "devices/Button.h"
#include "TimingTask.h"

class AlarmTask: public TimingTask {

public:
  AlarmTask(Button* pButton, TempSensorLM35* pTempSensor, Context* pContext); 
  void tick();
  void setFakeTemperature(int temp);
private:  
  void setState(ALARM_STATE state);
  ALARM_STATE state;
  TempSensorLM35* pTempSensor;
  Context* pContext;
  Button* pButton;
  bool waitingT3;
  bool waitingT4;
  int fakeTemperature;
};

#endif