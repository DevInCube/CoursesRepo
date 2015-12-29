#include <stdio.h>
#include <windows.h>

const char g_szClassName[] = "SampleWindow";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawPixels(HWND hwnd);

/*
IMPORTANT!
Compile with -l gdi32 for SetPixel()
*/
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
        CW_USEDEFAULT, CW_USEDEFAULT, 280, 220,
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
		case WM_PAINT:
			DrawPixels(hwnd);
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

void DrawPixels(HWND hwnd) {

	PAINTSTRUCT ps;
	RECT r;

	GetClientRect(hwnd, &r);

	if (r.bottom == 0)
	  return;

	HDC hdc = BeginPaint(hwnd, &ps);

	for (int i = 0; i < 1000; i++) {
	  int x = rand() % r.right;
	  int y = rand() % r.bottom;
	  SetPixel(hdc, x, y, RGB(rand() % 255, rand() % 255, rand() % 255));
	}
	Rectangle(hdc, 30, 30, 240, 140);
	
	HPEN hPen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN hPen2 = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
	HPEN hPen3 = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	HPEN hPen4 = CreatePen(PS_DASHDOT, 1, RGB(0, 0, 0));
	HPEN hPen5 = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 0));

	HPEN holdPen = SelectObject(hdc, hPen1);
	MoveToEx(hdc, 50, 30, NULL);
	LineTo(hdc, 200, 30);

	SelectObject(hdc, hPen2);
	MoveToEx(hdc, 50, 50, NULL);
	LineTo(hdc, 200, 50);

	SelectObject(hdc, hPen2);
	MoveToEx(hdc, 50, 70, NULL);
	LineTo(hdc, 200, 70);

	SelectObject(hdc, hPen3);
	MoveToEx(hdc, 50, 90, NULL);
	LineTo(hdc, 200, 90);

	SelectObject(hdc, hPen4);
	MoveToEx(hdc, 50, 110, NULL);
	LineTo(hdc, 200, 110);

	SelectObject(hdc, holdPen);
	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hPen3);
	DeleteObject(hPen4);
	DeleteObject(hPen5);

	EndPaint(hwnd, &ps);
}
