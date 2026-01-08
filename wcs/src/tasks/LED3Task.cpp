#include "tasks/LED3Task.h"
#include <Arduino.h>
#include "config.h"
#include "state.h"
#include "kernel/Logger.h"

LED3Task::LED3Task(Led* pLed, Context* pContext): 
    LEDTask(pLed, pContext){
    setState(LED_STATE::OFF);
}
  
void LED3Task::tick(){
    switch (state){   
         case LED_STATE::ON: {
            if (this->checkAndSetJustEntered()){
                pLed->switchOn();
                Logger.log(F("[LED3] ON"));
            }
            if (pContext->getAlarmState() != ALARM_STATE::ALARM){
                setState(LED_STATE::OFF);
            }
            break;
        }
        case LED_STATE::OFF: {
            if (this->checkAndSetJustEntered()){
                pLed->switchOff();
                Logger.log(F("[LED3] OFF"));
            }
            if (pContext->getAlarmState() == ALARM_STATE::ALARM){
                setState(LED_STATE::ON);
            }
            break;
        }
    }
}

