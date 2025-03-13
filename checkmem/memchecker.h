#pragma once
#include "offsets.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <algorithm>

class entity;

#define MAX_ENTITIES 64


namespace checkMemory {
    class memchecker;

    extern std::vector<entity*> entityVector;
    extern entity* localplayer;

    class memchecker {
    private:
        HANDLE pHandle;
        DWORD pid;
        volatile bool killSwitch;
    public:
        uintptr_t baseAddress;
        void initVars(LPCSTR pName);
        memchecker(volatile bool killSwitch);

        uintptr_t findGameBaseAddress();


        DWORD getPID() {
            if (pid == DWORD()) initVars("ac_client.exe");
            return this->pid;
        }
    
        inline volatile bool getKillSwitch() {
            return killSwitch;
        }
    
        HANDLE getProcessHandle() {
            if (pHandle == HANDLE()) initVars("ac_client.exe");
            return pHandle;
        }
    
        inline uintptr_t getGameBaseA() {
            return this->baseAddress;
        }
    
        std::vector<entity*> returnEntities() {
            return entityVector;
        }
    
        ~memchecker() {
            std::cout << "[DEBUG] Memory checker shutdown.\n";
        }
    };

    template <typename varType>
    varType rpm(uintptr_t address, int offset, checkMemory::memchecker* check) {
        varType buffer{};
        SIZE_T bytesRead;
    
        if (address == 0 || address < 0x10000) {
            std::cout << "[ERROR] Invalid address: " << std::hex << address << std::endl;
            return buffer;
        }
    
        bool success = ReadProcessMemory(check->getProcessHandle(), (LPCVOID)(address + offset), &buffer, sizeof(varType), &bytesRead);
        
        if (!success || bytesRead != sizeof(varType)) {
            DWORD errorCode = GetLastError();
            std::cout << "[ERROR] ReadProcessMemory failed! Address: " << std::hex << (address + offset)
                      << " | Error Code: " << std::dec << errorCode << std::endl;
        }
        return buffer;
    }



    template <typename varType> bool wpm(uintptr_t address, int offset, varType value, checkMemory::memchecker* check) {
        SIZE_T bytesWritten;

        std::cout << "[DEBUG] Writing " << value << " to " << std::hex << (address + offset) << std::dec << "\n";

        bool success = WriteProcessMemory(check->getProcessHandle(), (LPVOID)(address + offset), &value, sizeof(int), &bytesWritten);

        if (!success) {
            DWORD err = GetLastError();
            std::cout << "[ERROR] WriteProcessMemory failed! Error Code: " << err << "\n\n";
        } else {
            std::cout << "[DEBUG] Successfully wrote " << value << " (Bytes Written: " << bytesWritten << ")\n\n";
        }

        return success;
    }
};