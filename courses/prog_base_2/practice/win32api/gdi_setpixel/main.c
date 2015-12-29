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
        CW_USEDEFAULT, CW_USEDEFAULT, 680, 520,
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

	// pixels
	for (int i = 0; i < 1000; i++) {
	  int x = rand() % r.right;
	  int y = rand() % r.bottom;
	  SetPixel(hdc, x, y, RGB(rand() % 255, rand() % 255, rand() % 255));
	}
	
	// rectangle
	Rectangle(hdc, 30, 30, 240, 140);
	
	// pens and lines
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
	
	// brushes
	HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	holdPen = SelectObject(hdc, hPen);

	HBRUSH hBrush1 = CreateSolidBrush(RGB(121, 90, 0));
	HBRUSH hBrush2 = CreateSolidBrush(RGB(240, 63, 19));
	HBRUSH hBrush3 = CreateSolidBrush(RGB(240, 210, 18));
	HBRUSH hBrush4 = CreateSolidBrush(RGB(9, 189, 21));

	HBRUSH holdBrush = SelectObject(hdc, hBrush1);

	const int top = 200;
	const int size = 70;
	Rectangle(hdc, 30, top, 100, top+size);
	SelectObject(hdc, hBrush2);
	Rectangle(hdc, 110, top, 180, top+size);
	SelectObject(hdc, hBrush3);
	Rectangle(hdc, 30, top+70, 100, top+size+70);
	SelectObject(hdc, hBrush4);
	Rectangle(hdc, 110, top+70, 180, top+size+70);

	SelectObject(hdc, holdPen);
	SelectObject(hdc, holdBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
	DeleteObject(hBrush4);
	
	const int left = 370;
	Ellipse(hdc, left, 30, left + 100, 90);
	RoundRect(hdc, left+120, 30, left+240, 90, 15, 20);
	Chord(hdc, 270, 30, 360, 90, 270, 45, 360, 45);
	const POINT polygon[10] = { left+30, 145, left+85, 165, left+105, 
      110, left+65, 125, left+30, 105 };
	const POINT bezier[4] = {280, 160, 320, 160, 
      325, 110, 350, 110};
	Polygon(hdc, polygon, 5);
	PolyBezier(hdc, bezier, 4);


	EndPaint(hwnd, &ps);
}
