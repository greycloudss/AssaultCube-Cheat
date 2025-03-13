#pragma once
#include "features\aimbot.h"

namespace payload {
    class Payload {
        private:
        volatile bool killSwitch;
        
        public:
            Payload();
            ~Payload();
    };
}