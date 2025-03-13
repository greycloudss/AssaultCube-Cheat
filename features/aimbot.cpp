#include "aimbot.h"
#include <cmath>
#include <cfloat>
#include <iostream>

using namespace checkMemory;

namespace Aim {

    vector3 Aimbot::calculateDelta(vector3 lp, vector3 ent) {
        return { ent.x - lp.x, ent.y - lp.y, ent.z - lp.z };
    }

    angle Aimbot::calculateAngles(vector3 lp, vector3 ent) {
        vector3 delta = calculateDelta(lp, ent);
        angle angles;

        float hypotenuse = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
        float horizontalHypotenuse = sqrt(delta.x * delta.x + delta.z * delta.z);

        angles.yaw = atan2f(delta.z, delta.x) * (180.0f / M_PI);

        angles.pitch = atan2f(delta.y, horizontalHypotenuse) * (180.0f / M_PI);

        angles.yaw = fmod(angles.yaw + 180.0f, 360.0f);
        if (angles.yaw < 0) angles.yaw += 360.0f;
        angles.yaw -= 180.0f;

        angles.pitch = std::clamp(angles.pitch, -89.0f, 89.0f);

        return angles;
    }

    float Aimbot::calcDist(entity* ent) {
        vector3 entPos = ent->getPos();
        vector3 locPos = localplayer->getPos();
        return sqrt(pow(entPos.x - locPos.x, 2) + pow(entPos.y - locPos.y, 2) + pow(entPos.z - locPos.z, 2));
    }

    entity* Aimbot::getClosest() {
        entity* bestTarget = nullptr;
        float closestDist = FLT_MAX;

        for (entity* e : checker->returnEntities()) {
            if (!e || e->getHealth() <= 0) continue;

            float dist = calcDist(e);
            if (dist < closestDist) {
                closestDist = dist;
                bestTarget = e;
            }
        }
        return bestTarget;
    }

    void Aimbot::snapAngle() {
        if (checker->getKillSwitch() || checker->returnEntities().empty()) return;

        entity* target = getClosest();
        if (!target) {
            std::cout << "[ERROR] No valid target found!\n";
            return;
        }

        vector3 localHeadPos = localplayer->getHeadPos();
        vector3 targetHeadPos = target->getHeadPos();

        angle aimTo = calculateAngles(localHeadPos, targetHeadPos);

        if (std::isnan(aimTo.yaw) || std::isnan(aimTo.pitch)) {
            std::cout << "[ERROR] Invalid angle calculation!\n";
            return;
        }

        std::cout << "[DEBUG] Writing angles -> Yaw: " << aimTo.yaw << " Pitch: " << aimTo.pitch << "\n";

        bool yawWritten = wpm(localplayer->getPtr(), offsets::PLAYER_CAMERA_X, aimTo.yaw + 90, checker);
        bool pitchWritten = wpm(localplayer->getPtr(), offsets::PLAYER_CAMERA_Y, aimTo.pitch, checker);

        if (!yawWritten || !pitchWritten) {
            std::cout << "[ERROR] Failed to write angles to memory!\n";
            return;
        }

        float newYaw = rpm<float>(localplayer->getPtr(), offsets::PLAYER_CAMERA_X, checker);
        float newPitch = rpm<float>(localplayer->getPtr(), offsets::PLAYER_CAMERA_Y, checker);
        std::cout << "[DEBUG] Read-back -> Yaw: " << newYaw << " Pitch: " << newPitch << "\n";
    }

    Aimbot::Aimbot(memchecker* checker) : checker(checker) {}
};
