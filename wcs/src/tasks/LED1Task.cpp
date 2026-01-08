#include "tasks/LED1Task.h"
#include <Arduino.h>
#include "config.h"
#include "state.h"
#include "kernel/Logger.h"

LED1Task::LED1Task(Led* pLed, Context* pContext): 
    LEDTask(pLed, pContext){
    setState(LED_STATE::OFF);
}
  
void LED1Task::tick(){
    switch (state){   
        case LED_STATE::OFF: {
            if (this->checkAndSetJustEntered()){  
                pLed->switchOff();
                Logger.log(F("[LED1] OFF"));
            }
            if (pContext->getDroneState() == DRONE_STATE::REST){
                setState(LED_STATE::ON);
            }
            break;
        }
         case LED_STATE::ON: {
            if (this->checkAndSetJustEntered()){
                pLed->switchOn();
                Logger.log(F("[LED1] ON"));
            }
            if (pContext->getDroneState() != DRONE_STATE::REST){
                setState(LED_STATE::OFF);
            } 
            break;
        }
    }
}

