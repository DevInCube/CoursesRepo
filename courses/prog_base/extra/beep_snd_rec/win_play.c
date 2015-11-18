#ifdef WIN32

#include <windows.h>

int getch(void) {
    return getch();
}

void _beep(int freq, int millis) {
    Beep(freq, millis);
}
void _sleep(int millis) {
    Sleep(millis);
}

#endif // WIN32

