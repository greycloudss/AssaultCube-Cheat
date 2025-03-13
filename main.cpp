#include "payload.h"

payload::Payload* gptr_payload = nullptr;

void OpenDebugConsole() {
    AllocConsole();
    FILE* f;

    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    
    std::cout << "[DEBUG] Console allocated.\n";
}


DWORD WINAPI createPayload(LPVOID params) {
    gptr_payload = new payload::Payload();
    return gptr_payload == nullptr ? 1 : 0;
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        OpenDebugConsole();
        CreateThread(NULL, 0, createPayload, NULL, 0, NULL);
        break;
        
    case DLL_PROCESS_DETACH:
        delete gptr_payload;
        break;

    default:
        break;
    }
    return TRUE;
}
