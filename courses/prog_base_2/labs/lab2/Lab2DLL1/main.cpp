#include "main.h"

int compare(int a, int b) {
    return a == b;
}

void reaction(stack_t * a, stack_t * b) {
    while(stack_getCount(a) > 0) {
        stack_pop(a);
    }
    while(stack_getCount(b) > 0) {
        stack_pop(b);
    }
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
