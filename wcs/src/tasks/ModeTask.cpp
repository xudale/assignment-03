#include "ModeTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

ModeTask::ModeTask(Button* pButton, Context* pContext): 
    pButton(pButton), pContext(pContext){
}

void ModeTask::tick(){
    const MODE_STATE currentState = pContext->getModeState();
    Logger.log(String("dale ModeTask: Current Mode State = ") + (currentState == MODE_STATE::UNCONNECTED ? "UNCONNECTED" : currentState == MODE_STATE::AUTOMATIC ? "AUTOMATIC" : "MANUAL"));
    switch (currentState){    
        case MODE_STATE::UNCONNECTED : {
            Logger.log("Sync:Mode:UNCONNECTED");
            break;
        }
        case MODE_STATE::AUTOMATIC : {
            if (pButton->isPressed()){
                pContext->setModeState(MODE_STATE::MANUAL);
                pContext->setPotState(POT_STATE::ACTIVE);
                Logger.log("Sync:Mode:MANUAL");
            }
            break;
        }
        case MODE_STATE::MANUAL: {
            if (pButton->isPressed()){
                pContext->setModeState(MODE_STATE::AUTOMATIC);
                pContext->setPotState(POT_STATE::IDLE);
                Logger.log("Sync:Mode:AUTOMATIC");
            }
            break;
        }
    }
}




