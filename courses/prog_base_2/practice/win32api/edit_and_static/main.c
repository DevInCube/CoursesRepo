#include <windows.h>
#include <CommCtrl.h>

const char g_szClassName[] = "myWindowClass";

enum {
    STATIC_ID = 1,
    EDIT_ID
};

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
                   HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow
                  )
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    hInst = hInstance;

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
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hStatic, hEdit;
    static char buf[100];
    switch(msg)
    {
        case WM_CREATE:
            hStatic = CreateWindowEx(0,
                              WC_STATIC,
                              "Label",
                              WS_CHILD | WS_VISIBLE,
                              50, 50, 130, 23,
                              hwnd,
                              (HMENU)STATIC_ID,
                              hInst,
                              NULL);
            SetWindowText(hStatic, TEXT("Static"));
            hEdit = CreateWindowEx(0,
                              WC_EDIT,
                              "Edit me",
                              WS_CHILD | WS_VISIBLE | WS_BORDER,
                              50, 20, 130, 23,
                              hwnd,
                              (HMENU)EDIT_ID,
                              hInst,
                              NULL);
            break;
        case WM_COMMAND:
           if((HIWORD(wParam) == EN_CHANGE) &&  //notification
              (LOWORD(wParam) == EDIT_ID)) {    //your edit control ID
                GetWindowText(hEdit, buf, sizeof(buf));
                SetWindowText(hStatic, TEXT(buf));
            }
            break;
        case WM_CLOSE:
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
