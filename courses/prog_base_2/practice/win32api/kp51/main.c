#include <windows.h>
#include <CommCtrl.h>

const char g_szClassName[] = "myWindowClass";

enum {
    STATIC_ID = 1,
    BUTTON_ID,
    BTN_DEL_ID,
    EDIT_ID,
    LIST_ID,
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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
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
    static HWND hStatic, hButton, hButtonDel, hEdit, hList;
    static char buf[100];
    switch(msg)
    {
        case WM_CREATE:
            hButton = CreateWindowEx(0,
                              WC_BUTTON,
                              "Add",
                              WS_CHILD | WS_VISIBLE  | WS_TABSTOP | BS_DEFPUSHBUTTON,
                              50, 50, 130, 23,
                              hwnd, (HMENU)BUTTON_ID, hInst, NULL);
            hButtonDel = CreateWindowEx(0,
                              WC_BUTTON,
                              "Delete",
                              WS_CHILD | WS_VISIBLE  | WS_TABSTOP | BS_DEFPUSHBUTTON,
                              50, 80, 130, 23,
                              hwnd, (HMENU)BTN_DEL_ID, hInst, NULL);
            hStatic = CreateWindowEx(0,
                              WC_STATIC,
                              "",
                              WS_CHILD | WS_VISIBLE,
                              50, 110, 130, 23,
                              hwnd, (HMENU)STATIC_ID, hInst, NULL);
            SetWindowText(hStatic, TEXT("Static"));
            hEdit = CreateWindowEx(0,
                              WC_EDIT,
                              "Edit me",
                              WS_CHILD | WS_VISIBLE | WS_BORDER,
                              50, 20, 130, 23,
                              hwnd, (HMENU)EDIT_ID, hInst, NULL);
            hList = CreateWindowEx(0,
                              WC_LISTBOX,
                              "Listbox",
                              WS_CHILD | WS_VISIBLE | WS_BORDER,
                              190, 20, 130, 160,
                              hwnd, (HMENU)LIST_ID, hInst, NULL);
            break;
        case WM_COMMAND: {
                switch (LOWORD(wParam)) {
                    case BUTTON_ID: {
                        GetWindowText(hEdit, buf, sizeof(buf));
                        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buf);
                        LRESULT res = SendMessage(hList, LB_GETCOUNT, 0, 0);
                        sprintf(buf, "%i item(s)", res);
                        SetWindowText(hStatic, TEXT(buf));
                        break;
                    }
                    case BTN_DEL_ID: {
                        LRESULT res = SendMessage(hList, LB_GETCURSEL, 0, 0);
                        SendMessage(hList, LB_DELETESTRING, res, 0);
                        res = SendMessage(hList, LB_GETCOUNT, 0, 0);
                        sprintf(buf, "%i item(s)", res);
                        SetWindowText(hStatic, TEXT(buf));
                        break;
                    }
                    case LIST_ID: {
                        break;
                    }
                }
                break;
            }
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

/*#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include <CommCtrl.h>


const char g_szClassName[] = "myWnd";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

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

    hInst = hInstance;

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

enum {
    BUTTON_ID = 4,
    BUTTON2_ID,
    STATIC_ID,
    EDIT_ID
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND hButton, hButton2, hStatic, hEdit;
    switch(msg)
    {
        case WM_CREATE: {
            hButton = CreateWindowEx(0,
                WC_BUTTON,
                "Click me plz!",
                WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                10, 10, 130, 23,
                hwnd,
                (HMENU)BUTTON_ID,
                hInst,
                NULL);
            hButton2 = CreateWindowEx(0,
                WC_BUTTON,
                "Button 2",
                WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                10, 40, 130, 23,
                hwnd,
                (HMENU)BUTTON2_ID,
                hInst,
                NULL);
            hStatic = CreateWindowEx(0,
                WC_STATIC,
                "Some text",
                WS_CHILD | WS_VISIBLE,
                10, 80, 130, 23,
                hwnd,
                (HMENU)STATIC_ID,
                hInst,
                NULL);
            hEdit = CreateWindowEx(0,
                WC_EDIT,
                "Edit me",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                10, 120, 130, 23,
                hwnd,
                (HMENU)EDIT_ID,
                hInst,
                NULL);
            break;
        }
        case WM_COMMAND : {
            int buttonID = LOWORD(wParam);
            int hiWord = HIWORD(wParam);
            if (buttonID == BUTTON_ID) {
                printf("Set\r\n");
                SetWindowText(hStatic, TEXT("Good boy!"));
            }
            if (buttonID == BUTTON2_ID) {
                char text[100];
                GetWindowText(hEdit, text, sizeof(text));
                SetWindowText(hStatic, TEXT(text));
            }
            printf("Command on %i with %i! lParam %i %i \r\n", buttonID, hiWord,
                   HIWORD(lParam), LOWORD(lParam));
            break;
        }
        case WM_MOUSEMOVE: {
            //CreateWindowIII();
            //printf("Move ");
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
*/
