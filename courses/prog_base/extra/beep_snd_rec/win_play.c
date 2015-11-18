#ifdef WIN32

#include <windows.h>

void _beep(int freq, int millis) {
    Beep(freq, millis);
}
void _sleep(int millis) {
    Sleep(millis);
}

#endif // WIN32

