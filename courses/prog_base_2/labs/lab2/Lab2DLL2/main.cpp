#include "main.h"

int compare(int a, int b) {
    return (a + b) > 10;
}

void reaction(stack_t * a, stack_t * b) {
    if (stack_peek(a) > stack_peek(b)) {
        stack_pop(a);
    } else {
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
