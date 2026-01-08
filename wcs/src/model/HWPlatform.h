#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/Led.h"
#include "devices/ServoMotor.h"
#include "devices/TempSensorLM35.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  Led* getLed(int index);
  ServoMotor* getMotor();
  TempSensorLM35* getTempSensor();
  Sonar* getSonar();
  Pir* getPir();

private:
  Button* pButton;
  Led* pLeds[3];
  ServoMotor* pMotor;
  TempSensorLM35* pTempSensor;
  Sonar* pSonar;
  Pir* pPir;
  
};

#endif
