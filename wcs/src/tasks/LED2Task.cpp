#include "tasks/LED2Task.h"
#include <Arduino.h>
#include "config.h"
#include "state.h"
#include "kernel/Logger.h"

LED2Task::LED2Task(Led* pLed, Context* pContext): 
    LEDTask(pLed, pContext){
    setState(LED_STATE::IDLE);
}
  
void LED2Task::tick(){
    switch (state){   
        case LED_STATE::IDLE: {
            if (this->checkAndSetJustEntered()){  
                pLed->switchOff();
            }
            if (pContext->getDroneState() == DRONE_STATE::TAKING_OFF ||
                pContext->getDroneState() == DRONE_STATE::LANDING){
                setState(LED_STATE::ON);
            }
            break;
        }
        case LED_STATE::ON: {
            if (this->checkAndSetJustEntered()){
                pLed->switchOn();
            }
            if (pContext->getDroneState() == DRONE_STATE::TAKING_OFF ||
                pContext->getDroneState() == DRONE_STATE::LANDING){
                setState(LED_STATE::OFF);
            } else {
                setState(LED_STATE::IDLE);
            }
            break;
        }
        case LED_STATE::OFF: {
            if (this->checkAndSetJustEntered()){  
                pLed->switchOff();
            }
            if (pContext->getDroneState() == DRONE_STATE::TAKING_OFF ||
                pContext->getDroneState() == DRONE_STATE::LANDING){
                setState(LED_STATE::ON);
            } else {
                setState(LED_STATE::IDLE);
            }
            break;
        }
    }
}

