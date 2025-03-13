#pragma once
#include "memchecker.h"

class entity {
    private:
        uintptr_t ent_ptr;
        checkMemory::memchecker* check;
        vector3 pos;
        vector3 headPos;
        int health;
        
    public:
        entity(uintptr_t ent_ptr, checkMemory::memchecker* check);

        inline int getHealth() {
            return health;
        }

        inline uintptr_t getPtr() const {
            return ent_ptr;
        }

        inline vector3 getHeadPos() {
            return headPos;
        }
        
        inline vector3 getPos() {
            return pos;
        }
};
