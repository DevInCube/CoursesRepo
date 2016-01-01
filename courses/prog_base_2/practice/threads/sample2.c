#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <conio.h>

#define LIMIT_VALUE 10

DWORD producer(LPVOID);
DWORD consumer(LPVOID);

HANDLE newThread(LPTHREAD_START_ROUTINE routine, int * ctx);

int main(void) {
    // shared resource
    int value = 0;
    // create new threads
    HANDLE consumers[] = {
        newThread(consumer, &value),
        newThread(consumer, &value),
        newThread(consumer, &value),
        newThread(consumer, &value),
    };
    HANDLE producers[] = {
        newThread(producer, &value),
        newThread(producer, &value),
    };
    // wait in main thread
    while (!_kbhit()) {
        system("cls");
        printf("Value is: %i", value);
        fflush(stdout);
        Sleep(10);
    }
    // free
    for (int i = 0; i < 2; i++) {
        CloseHandle(producers[i]);
    }
    for (int i = 0; i < 4; i++) {
        CloseHandle(consumers[i]);
    }
    _getch();
    system("cls");
    return 0;
}

DWORD producer(LPVOID args) {
    int * param = (int *)args;
    while (TRUE) {
        if (*param < LIMIT_VALUE) {
            (*param)++;
            if (*param >= LIMIT_VALUE) {
                printf("\n\nERROR, value is %i", *param);
                exit(1);        // HERE!
            }
        }
        Sleep(30);
    }
}
DWORD consumer(LPVOID args) {
    int * param = (int *)args;
    while (TRUE) {
        if (*param > 0) {
            (*param)--;
            if (*param < 0) {
                printf("\n\nERROR, value is %i", *param);
                exit(1);        // HERE!
            }
        }
        Sleep(20);
    }
}

HANDLE newThread(LPTHREAD_START_ROUTINE routine, int * ctx) {
    return CreateThread(
        NULL,               // default security attributes
        0,                  // default stack size
        (LPTHREAD_START_ROUTINE) routine,  // thread function
        (LPVOID)ctx,        // thread function arguments
        0,                  // default creation flags
        NULL);              // receive thread identifier
}
