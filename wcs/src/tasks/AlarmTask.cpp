#include "AlarmTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

AlarmTask::AlarmTask(Button* pButton, TempSensorLM35* pTempSensor, Context* pContext): 
    pButton(pButton), pTempSensor(pTempSensor), pContext(pContext){
    setState(ALARM_STATE::NORMAL);
    fakeTemperature = -1;
}

void AlarmTask::tick(){
    int temperature;
    if (pButton->isPressed()){
        setFakeTemperature(-1);
    }
    if (fakeTemperature >= 0) {
        temperature = fakeTemperature;
    } else {
        temperature = (int)pTempSensor->getTemperature();
    }
    Logger.log("AlarmTask: Temperature = " + String(temperature));
    switch (state){    
        case ALARM_STATE::NORMAL: {
            if (temperature >= TEMP1) {
                if (!waitingT3) {
                    waitingT3 = true;
                    stateTimestamp = millis();
                } 
                if (waitingT3 && elapsedTimeInState() >= T3) {
                    setState(ALARM_STATE::PRE_ALARM);
                }
            } else {
                waitingT3 = false;
            }
            break;
        }
        case ALARM_STATE::PRE_ALARM : {
            if (temperature >= TEMP2) {
                if (!waitingT4) {
                    waitingT4 = true;
                    stateTimestamp = millis();
                } 
                if (waitingT4 && elapsedTimeInState() >= T4) {
                    setState(ALARM_STATE::ALARM);   
                }
                
            } else if (temperature < TEMP1) {
                setState(ALARM_STATE::NORMAL);
            } else {
                waitingT4 = false;
            }
            break;
        }
        case ALARM_STATE::ALARM: {
            if (temperature < TEMP2) {
                setState(ALARM_STATE::PRE_ALARM);
            }
            break;
        }
    }
}

void AlarmTask::setFakeTemperature(int temp){
    this->fakeTemperature = temp;
}

void AlarmTask::setState(ALARM_STATE s){
    state = s;
    pContext->setAlarmState(s);
    waitingT3 = false;
    waitingT4 = false;
    stateTimestamp = millis();
}


