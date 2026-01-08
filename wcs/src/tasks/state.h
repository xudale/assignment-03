#ifndef __STATE__
#define __STATE__

enum class DRONE_STATE : uint8_t {
    REST, TAKING_OFF, OUT, LANDING, OUT_WITH_ALARM
};

enum class HANGAR_DOOR_STATE : uint8_t {
    CLOSED, OPEN 
};

enum class ALARM_STATE : uint8_t {
    NORMAL, PRE_ALARM, ALARM
};

enum class LED_STATE : uint8_t {
    IDLE, OFF, ON
};
#endif