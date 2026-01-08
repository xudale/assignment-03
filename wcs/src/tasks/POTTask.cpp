#include "POTTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

POTTask::POTTask(Potentiometer* pPOT, ServoMotor *pMotor, Context* pContext): 
   pPOT(pPOT), pMotor(pMotor), pContext(pContext){
}

void POTTask::tick(){
    const POT_STATE currentState = pContext->getPotState();
    switch (currentState){    
        case POT_STATE::IDLE: {
            break;
        }
        case POT_STATE::ACTIVE : {
            pPOT->sync();
            float potValue = pPOT->getValue();
            int angle = (int)(potValue * 90); // Scale to 0-90 degrees
            Logger.log("dale POTTask: Potentiometer value = " + String(potValue) + ", setting motor angle to " + String(angle));
            pMotor->on();
            pMotor->setPosition(angle);
            break;
        }        
    }
}


