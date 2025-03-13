#include "entity.h"

using namespace checkMemory;

entity::entity(uintptr_t ent_ptr, checkMemory::memchecker* check) : ent_ptr(ent_ptr),  check(check) {
    this->health = rpm<float>(ent_ptr, offsets::HEALTH_VALUE, check);

    this->pos = vector3(rpm<float>(ent_ptr, offsets::POSITION_X, check),
     rpm<float>(ent_ptr, offsets::POSITION_Y, check),
     rpm<float>(ent_ptr, offsets::POSITION_Z, check));

    this->headPos = vector3(rpm<float>(ent_ptr, offsets::HEAD_POSITION_X, check),
    rpm<float>(ent_ptr, offsets::HEAD_POSITION_Y, check),
    rpm<float>(ent_ptr, offsets::HEAD_POSITION_Z, check));

    health = rpm<int>(ent_ptr, offsets::HEALTH_VALUE, check);
}