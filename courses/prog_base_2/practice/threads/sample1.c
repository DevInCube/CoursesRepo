#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <conio.h>

struct Context {
    int a;
    int b;
};

DWORD threadUpdateA(LPVOID);
DWORD threadUpdateB(LPVOID);
DWORD threadDraw(LPVOID);

HANDLE newThread(LPTHREAD_START_ROUTINE routine, struct Context * ctx) {
    return CreateThread(
        NULL,               // default security attributes
        0,                  // default stack size
        (LPTHREAD_START_ROUTINE) routine,  // thread function
        (LPVOID)ctx,        // thread function arguments
        0,                  // default creation flags
        NULL);              // receive thread identifier
}

int main(void) {
    // shared resource
    struct Context context = {0, 0};
    // create new threads
    HANDLE hThreads[] = {
        newThread(threadUpdateA, &context),
        newThread(threadUpdateB, &context),
        newThread(threadDraw, &context)
    };
    // wait in main thread
    _getch();
    // free
    for (int i = 0; i < 3; i++) {
        CloseHandle(hThreads[i]);
    }
    system("cls");
    return 0;
}

DWORD threadUpdateA(LPVOID args) {
    struct Context * ctx = (struct Context *)args;
    while (TRUE) {
        ctx->a++;
        Sleep(20);
    }
}
DWORD threadUpdateB(LPVOID args) {
    struct Context * ctx = (struct Context *)args;
    while (TRUE) {
        ctx->b += 2;
        Sleep(20);
    }
}
DWORD threadDraw(LPVOID args) {
    struct Context * ctx = (struct Context *)args;
    while (TRUE) {
        system("cls");
        printf("A: %i\nB: %i\n", ctx->a, ctx->b);
        fflush(stdout);
        Sleep(20);
    }
}
