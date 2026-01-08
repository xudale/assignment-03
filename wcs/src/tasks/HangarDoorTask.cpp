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
            if (pContext->getDroneState() == DRONE_STATE::LANDING || 
                pContext->getDroneState() == DRONE_STATE::TAKING_OFF) {
                setState(HANGAR_DOOR_STATE::OPEN);
            }
            break;
        }
        case HANGAR_DOOR_STATE::OPEN : {
            if (pContext->getDroneState() == DRONE_STATE::TAKING_OFF &&
                       distance > D1) {
                waitingT2 = false;
                if (!waitingT1) {
                    waitingT1 = true;
                    stateTimestamp = millis();
                }
                if (waitingT1 && elapsedTimeInState() >= T1) { 
                    setState(HANGAR_DOOR_STATE::CLOSED);    
                    pContext->setDroneState(DRONE_STATE::OUT);    
                }
            } else if (pContext->getDroneState() == DRONE_STATE::LANDING &&
                       distance < D2) {
                waitingT1 = false; 
                if (!waitingT2) {
                    waitingT2 = true;
                    stateTimestamp = millis();
                }
                if (waitingT2 && elapsedTimeInState() >= T2 && pPir->isDetected()) { 
                    setState(HANGAR_DOOR_STATE::CLOSED);    
                    pContext->setDroneState(DRONE_STATE::REST);    
                }
            } else {
                waitingT1 = false;
                waitingT2 = false; 
            }
            break;
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


