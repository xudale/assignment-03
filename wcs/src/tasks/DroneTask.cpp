#include "DroneTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

DroneTask::DroneTask(Context* pContext): 
    pContext(pContext){
}

void DroneTask::tick(){
    const DRONE_STATE currentState = pContext->getDroneState();
    switch (currentState){    
        case DRONE_STATE::REST: {
            break;
        }
        case DRONE_STATE::TAKING_OFF : {
            if (pContext->getAlarmState() == ALARM_STATE::ALARM) {
                setState(DRONE_STATE::REST);
            } 
            break;
        }
        case DRONE_STATE::OUT: {
            if (pContext->getAlarmState() == ALARM_STATE::ALARM) {
                setState(DRONE_STATE::OUT_WITH_ALARM);
            }
            break;
        }
        case DRONE_STATE::OUT_WITH_ALARM: {
            if (pContext->getAlarmState() != ALARM_STATE::ALARM) {
                setState(DRONE_STATE::OUT);
            }
            break;
        }
        case DRONE_STATE::LANDING: {
            if (pContext->getAlarmState() == ALARM_STATE::ALARM) {
                setState(DRONE_STATE::REST);
            } 
            break;
        }
    }
}

void DroneTask::setState(DRONE_STATE s){
    pContext->setDroneState(s);
}


