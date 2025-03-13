#include "payload.h"

checkMemory::memchecker* gptr_mem = nullptr;
using namespace payload;

Payload::Payload() :killSwitch(false) {
    gptr_mem = new checkMemory::memchecker(killSwitch);

    MessageBoxA(NULL, "Return pressed", "Success", MB_OK);
}
Payload::~Payload() {
    std::cout << "[Payload] Destroyed!\n";
}