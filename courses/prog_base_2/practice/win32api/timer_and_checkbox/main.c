#include <stdio.h>
#include <windows.h>

const char g_szClassName[] = "SampleWindow";

#define ID_LABEL 131
#define ID_TIMER 132
#define ID_CB    133

static int ticks = 0;

// Step 4: the Window Procedure
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
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 220,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
    switch(msg)
    {
		case WM_CREATE:
		{
			CreateWindowW(
				L"button", 
				L"Update timer",
				WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX,	// BS_AUTOCHECKBOX to handle user input!
				20, 40, 185, 35, 
				hwnd, (HMENU) ID_CB, NULL, NULL
				);

			CheckDlgButton(hwnd, ID_CB, BST_CHECKED);
			
			HWND hLabel = CreateWindowEx(
				0,
				"STATIC",
				"Default",
				 WS_CHILD | WS_VISIBLE,
				10, 10, 100, 26,
				hwnd, (HMENU)ID_LABEL, hInstance, NULL
				);
			
			const int TIMER_TICK = 1000;  // in ms
			int ret = SetTimer(hwnd, ID_TIMER, TIMER_TICK, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		break;
		case WM_TIMER:
		{
			int checked = IsDlgButtonChecked(hwnd, ID_CB);
			if (checked) {
				ticks++;
				char buf[256] = "";
				sprintf(buf, "Ticks: %i", ticks);
				
				HWND hLabel = GetDlgItem(hwnd, ID_LABEL);
				SendMessage(hLabel, WM_SETTEXT , (WPARAM)256, (LPARAM)buf);
			}
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
