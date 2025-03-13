#include  "..\features\aimbot.h"

Aim::Aimbot* gptr_aim = nullptr;

namespace checkMemory {
    std::vector<entity*> entityVector;
    entity* localplayer = nullptr;

    inline void updateEntityList(uintptr_t entityList, checkMemory::memchecker* checker) {
        std::vector<entity*> tmp;

        if (rpm<int>(checker->baseAddress, offsets::PLAYER_COUNT_OFFSET, checker) == 0) return;

        uintptr_t entityListPtr = rpm<uintptr_t>(checker->baseAddress, offsets::ENTITY_LIST_OFFSET, checker);
        if (entityListPtr == 0 || entityListPtr < 0x10000) {
            std::cout << "[ERROR] Invalid Entity List Address: " << std::hex << entityListPtr << std::endl;
            return;
        }

        int entityCount = rpm<int>(checker->baseAddress, 0x0018AC0C, checker);
        std::cout << "[DEBUG] Entity Count: " << entityCount << std::endl;

        for (int i = 0; i < entityCount; ++i) {
            uintptr_t entity_ptr = rpm<uintptr_t>(entityListPtr, i * sizeof(uintptr_t), checker);

            if (entity_ptr == 0 || entity_ptr < 0x10000) {
                std::cout << "[DEBUG] Skipping invalid entity pointer at index: " << i << std::endl;
                continue;
            }

            MEMORY_BASIC_INFORMATION mbi;
            if (VirtualQueryEx(checker->getProcessHandle(), (LPCVOID)entity_ptr, &mbi, sizeof(mbi)) == 0) {
                std::cout << "[ERROR] VirtualQueryEx failed! Address: " << std::hex << entity_ptr << " | Error Code: " << GetLastError() << std::endl;
                continue;
            }

            if (mbi.State != MEM_COMMIT) {
                std::cout << "[DEBUG] Skipping non-committed memory at: " << std::hex << entity_ptr << std::endl;
                continue;
            }

            int entHealth = rpm<int>(entity_ptr, offsets::HEALTH_VALUE, checker);
            if (entHealth > 100 || entHealth < 0) {
                std::cout << "[DEBUG] Skipping entity with invalid health: " << entHealth << "\n";
                continue;
            }

            tmp.push_back(new entity(entity_ptr, checker));
        }

        for (entity* e : entityVector) delete e;  // Free old entities
        entityVector.clear();
        entityVector = tmp;

        std::cout << "[DEBUG] Updated entity list, new size: " << entityVector.size() << std::endl;
    }

    DWORD WINAPI update(LPVOID param) {
        checkMemory::memchecker* checker = (checkMemory::memchecker*) param;

        while (!checker->getKillSwitch()) {
            uintptr_t playerBase = rpm<uintptr_t>(checker->baseAddress, offsets::LOCAL_PLAYER_OFFSET, checker);
            if (localplayer) delete localplayer;  // Free old pointer
            localplayer = new entity(playerBase, checker);

            updateEntityList(rpm<uintptr_t>(checker->baseAddress, offsets::ENTITY_LIST_OFFSET, checker), checker);

            std::cout << "[Debug] entityVector size: " << entityVector.size() << "\n";

            wpm(playerBase, offsets::HEALTH_VALUE, 19, checker);
            std::cout << "[DEBUG] localplayer health: " << rpm<int>(playerBase, offsets::HEALTH_VALUE, checker) << "\n";

            Sleep(50);
        }

        return 0;
    }

    uintptr_t memchecker::findGameBaseAddress() {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
        if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(MODULEENTRY32);
        uintptr_t baseAddr = 0;

        if (Module32First(hSnapshot, &modEntry)) {
            do {
                if (!strcmp((const char*)modEntry.szModule, "ac_client.exe")) {
                    baseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &modEntry));
        }

        CloseHandle(hSnapshot);
        return baseAddr;
    }

    void memchecker::initVars(LPCSTR pName) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);

        if (Process32First(snapshot, &pEntry)) {
            do {
                if (strcmp((const char*)pEntry.szExeFile, pName) == 0) {
                    CloseHandle(snapshot);
                    this->pid = pEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &pEntry));
        }

        CloseHandle(snapshot);
        pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    memchecker::memchecker(volatile bool killSwitch) : killSwitch(killSwitch) {
        getPID();
        baseAddress = findGameBaseAddress();
        CreateThread(NULL, 0, update, (LPVOID)this, 0, NULL);
        gptr_aim = new Aim::Aimbot(this);
        while (!killSwitch) {
            gptr_aim->snapAngle();
            Sleep(5);
        }
    }
};
