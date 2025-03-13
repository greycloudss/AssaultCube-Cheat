#pragma once
#include "..\checkmem\entity.h"
#include "..\checkmem\offsets.h"

struct angle {
    float pitch, yaw;
    angle() = default;
    angle(float pitch, float yaw) : pitch(pitch), yaw(yaw) {}
};

using namespace checkMemory;


namespace Aim {
    class Aimbot{
        private:
        memchecker* checker;
        entity* closestEntity;

        public:
        Aimbot(memchecker* checker);

        vector3 calculateDelta(vector3 lp, vector3 ent);
        angle calculateAngles(vector3 lp, vector3 ent);

        float calcDist(entity* ent);
        entity* getClosest();
        void snapAngle();

        ~Aimbot() = default;
    };
};
