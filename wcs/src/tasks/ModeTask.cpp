#include "ModeTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

ModeTask::ModeTask(Button* pButton, Context* pContext): 
    pButton(pButton), pContext(pContext){
}

void ModeTask::tick(){
    const MODE_STATE currentState = pContext->getModeState();
    switch (currentState){    
        case MODE_STATE::UNCONNECTED : {
            Logger.log(F("Sync:Mode:UNCONNECTED"));
            break;
        }
        case MODE_STATE::AUTOMATIC : {
            if (pButton->isPressed()){
                pContext->setModeState(MODE_STATE::MANUAL);
                pContext->setPotState(POT_STATE::ACTIVE);
                Logger.log(F("Sync:Mode:MANUAL"));
            }
            break;
        }
        case MODE_STATE::MANUAL: {
            if (pButton->isPressed()){
                pContext->setModeState(MODE_STATE::AUTOMATIC);
                pContext->setPotState(POT_STATE::IDLE);
                Logger.log(F("Sync:Mode:AUTOMATIC"));
            }
            break;
        }
    }
}




