
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

const char g_szClassName[] = "myWnd";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hIns;

int WINAPI WinMain(
                   HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow
                  )
{
    //srand();
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    hIns = hInstance;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONINFORMATION | MB_OKCANCEL);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "myWnd",
        "The cat",
        WS_OVERLAPPEDWINDOW ,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 200,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

void CreateWindowIII(void) {
    MSG Msg;

    int cxScreen, cyScreen ;
    cxScreen = GetSystemMetrics (SM_CXSCREEN) ;
    cyScreen = GetSystemMetrics (SM_CYSCREEN) ;

    const int width = 240, height = 200;

    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "myWnd",
        "The cat",
        WS_OVERLAPPEDWINDOW ,
        rand() % (cxScreen - width), rand() % (cyScreen - height) , width, height,
        NULL, NULL, hIns, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

    switch(msg)
    {
        case WM_MOUSEMOVE: {
            CreateWindowIII();
            printf("Move\r\n");
            break;
        }
        case WM_CLOSE:
            printf("CLose!");
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
