#include "HangarDoorTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

HangarDoorTask::HangarDoorTask(Pir* pPir ,Sonar* pSonar, ServoMotor *pMotor, Context* pContext): 
    pPir(pPir),pSonar(pSonar), pMotor(pMotor),pContext(pContext){
    setState(HANGAR_DOOR_STATE::CLOSED);
    waitingT1 = false;
    waitingT2 = false;
}

void HangarDoorTask::tick(){
    int distance = (int)pSonar->getDistance();
    pContext->setDistance(distance);
    pPir->sync();
    switch (state){    
        case HANGAR_DOOR_STATE::CLOSED: {
           
        }
        case HANGAR_DOOR_STATE::OPEN : {
           
        }
    }
}

void HangarDoorTask::setState(HANGAR_DOOR_STATE s){
    state = s;
    pContext->setHangarDoorState(s);
    waitingT1 = false;
    waitingT2 = false;
    stateTimestamp = millis();
    if (state == HANGAR_DOOR_STATE::OPEN) {
        pMotor->on();
        pMotor->setPosition(0);
    } else if (state == HANGAR_DOOR_STATE::CLOSED) {
        pMotor->on();
        pMotor->setPosition(180);
    }
}


