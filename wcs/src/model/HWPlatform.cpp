#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/Led.h"
#include "devices/ServoMotorImpl.h"
#include "kernel/Logger.h"

void wakeUp(){}

HWPlatform::HWPlatform(){
  pButton = new ButtonImpl(BT_PIN);
  pLeds[0] = new Led(LED1_PIN);
  pLeds[1] = new Led(LED2_PIN);
  pLeds[2] = new Led(LED3_PIN);
  pMotor = new ServoMotorImpl(MOTOR_PIN);
  pTempSensor = new TempSensorLM35(TEMPERATURE_PIN);
  pSonar = new Sonar(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN, 30000);
  pPir = new Pir(PIR_PIN);
}


void HWPlatform::init(){
}

Button* HWPlatform::getButton(){
  return this->pButton;
}

Led* HWPlatform::getLed(int index) {
  if (index < 3) {
      return this->pLeds[index];
  } else {
      return nullptr;
  }
}

ServoMotor* HWPlatform::getMotor(){
  return this->pMotor;
}

Sonar* HWPlatform::getSonar() {
  return this->pSonar;
}

Pir* HWPlatform::getPir() {
  return this->pPir;
}

TempSensorLM35* HWPlatform::getTempSensor() {
  return this->pTempSensor;
}

void HWPlatform::test(){
  bool btPressed = pButton->isPressed();
  pMotor->on();
  pMotor->setPosition(90);
  Logger.log("Button: " + String(btPressed ? "pressed" : " not pressed"));
  delay(1000);
  pMotor->setPosition(0);
  delay(1000);
  pMotor->off();
}

