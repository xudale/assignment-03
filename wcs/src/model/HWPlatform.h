#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/ServoMotor.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "devices/Pot.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  ServoMotor* getMotor();
  Sonar* getSonar();
  Pir* getPir();
  Potentiometer* getPot();

private:
  Button* pButton;
  ServoMotor* pMotor;
  Sonar* pSonar;
  Pir* pPir;
  Potentiometer* pPot;
  
};

#endif
