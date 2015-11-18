#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "termio.h"

int _getch(void) {
    return getch();
}

void _beep(int freq, int ms) {
  char command[20];
  sprintf(command, "sh alarm.sh %i %i", freq, ms);
  system(command);
}

void _sleep(int millis) {
    usleep(millis);
}

#endif // __linux__
