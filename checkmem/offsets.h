#pragma once
#include <windows.h>

class vector3 {
    public:
    float x, y, z;
    vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    vector3(float* arr) {
        this->x = arr == nullptr ? 0.0f : arr[0];
        this->y = arr == nullptr ? 0.0f : arr[1];
        this->z = arr == nullptr ? 0.0f : arr[2];
    }
};


namespace offsets {
    constexpr uintptr_t LOCAL_PLAYER_OFFSET      = 0x0017E0A8;
    constexpr uintptr_t ENTITY_LIST_OFFSET       = 0x18AC04;  
    constexpr uintptr_t FOV_OFFSET               = 0x18A7CC;  
    constexpr uintptr_t PLAYER_COUNT_OFFSET      = 0x18AC0C;
    constexpr uintptr_t CMD_OFFSET               = 0x328E60;

    constexpr uintptr_t POSITION_X               = 0x2C;
    constexpr uintptr_t POSITION_Y               = 0x30;
    constexpr uintptr_t POSITION_Z               = 0x28;

    constexpr uintptr_t HEAD_POSITION_X          = 0x4;
    constexpr uintptr_t HEAD_POSITION_Y          = 0xC;
    constexpr uintptr_t HEAD_POSITION_Z          = 0x8;

    constexpr uintptr_t PLAYER_CAMERA_X          = 0x34;
    constexpr uintptr_t PLAYER_CAMERA_Y          = 0x38;

    constexpr uintptr_t AUTO_SHOOT               = 0x204;
    constexpr uintptr_t HEALTH_VALUE             = 0xEC;
    constexpr uintptr_t ARMOR_VALUE              = 0xF0;
    constexpr uintptr_t PLAYER_NAME              = 0x205;  
}