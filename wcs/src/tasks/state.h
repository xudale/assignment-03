#ifndef __STATE__
#define __STATE__

enum class MODE_STATE : uint8_t {
    UNCONNECTED, AUTOMATIC, MANUAL
};

enum class POT_STATE : uint8_t {
    IDLE, ACTIVE
};

enum class HANGAR_DOOR_STATE : uint8_t {
    CLOSED, OPEN 
};

#endif