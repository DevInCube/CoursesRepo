#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <conio.h>

const int SLEEP_TIME = 30;

HANDLE hMutex;

DWORD threadRun(LPVOID);    // LPVOID is void *
void mainRun(void);         // main thread function

int main(void) {
    hMutex = CreateMutex(
        NULL,               // default security attributes
        FALSE,              // initially not owned
        NULL);              // unnamed mutex
    DWORD tId;
    HANDLE hReadThread = CreateThread(
        NULL,               // default security attributes
        0,                  // default stack size
        (LPTHREAD_START_ROUTINE) threadRun,
        NULL,               // no thread function arguments
        0,                  // default creation flags
        &tId);              // receive thread identifier

    mainRun();
    // close resources
    CloseHandle(hReadThread);
    CloseHandle(hMutex);

    system("cls");
    return 0;
}

void putAt(char ch, int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hConsole, pos);
    putchar(ch);
}

void drawRect(int px, int py) {
    int i;
    WaitForSingleObject(hMutex, INFINITE);
    // SYNC {
    for (i = 0; i < 9; i++) {
        putAt(((i == 4) ? ' ' : '@'), (px - 1) + (i % 3), (py - 1) + (i / 3));
    }
    // }
    ReleaseMutex(hMutex);
}

void mainRun(void) {
    char key = 0;
    int px = 40, py = 12;
    drawRect(px, py);
    do {
        if (_kbhit()) {
            key = _getch();
            switch(key) {
            case 'w':
                if (py > 1) {
                    py--;
                }
                break;
            case 'a':
                if (px > 12) {
                    px--;
                }
                break;
            case 's':
                if (py < 23) {
                    py++;
                }
                break;
            case 'd':
                if (px < 67) {
                    px++;
                }
                break;
            }
            drawRect(px, py);
        }
        Sleep(SLEEP_TIME);
    } while ('q' != key);
}

void drawAnimation(int index, int left, int top, int right, int bottom, int invert) {
    int x, y, width = right - left;
    // clear area
    for (x = left; x < right; x++) {
        for (y = top; y < bottom; y++) {
            putAt(' ', x, y);
        }
    }
    // draw
    for (y = top; y < bottom; y++) {
        x = left + (invert ? ((width) - (index + y) % width) : ((index + y) % width));
        putAt('#', x, y);
    }
}

DWORD threadRun(LPVOID args) {
    int index = 0, y;

    WaitForSingleObject(hMutex, INFINITE);
    // SYNC {
    for (y = 0; y < 24; y++) {
        putAt('#', 10, y);
        putAt('#', 69, y);
    }
    // }
    ReleaseMutex(hMutex);

    while (TRUE) {

        WaitForSingleObject(hMutex, INFINITE);
        // SYNC {
        drawAnimation(index, 70, 0, 80, 24, 0);
        drawAnimation(index, 00, 0, 10, 24, 1);
        // }
        ReleaseMutex(hMutex);

        index++;
        Sleep(SLEEP_TIME);
    }
}
