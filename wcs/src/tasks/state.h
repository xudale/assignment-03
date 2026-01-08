#ifndef __STATE__
#define __STATE__

enum class DRONE_STATE : uint8_t {
    REST, TAKING_OFF, OUT, LANDING, OUT_WITH_ALARM
};

enum class MODE_STATE : uint8_t {
    UNCONNECTED, AUTOMATIC, MANUAL
};

enum class POT_STATE : uint8_t {
    IDLE, ACTIVE
};

enum class HANGAR_DOOR_STATE : uint8_t {
    CLOSED, OPEN 
};

enum class ALARM_STATE : uint8_t {
    NORMAL, PRE_ALARM, ALARM
};

#endif