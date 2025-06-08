#include <Windows.h>

#define WIDTH    512
#define HEIGHT   512
#define TIMER_ID 1

DWORD randseed = 0x12345678;

void my_randomize() {
	randseed ^= GetTickCount();
}

DWORD my_random(DWORD range) {
	randseed = randseed * 214013 + 2531011;
	if (range >= 65536)
		return randseed % range;
	else
		return ((randseed >> 16) * range) >> 16;
}

DWORD random_dword() {
	return  ((1+my_random(255))<<24) |
			((1+my_random(255))<<16) |
			((1+my_random(255))<< 8) |
			(1+my_random(255)) ;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;

	switch (msg) {
	case WM_DESTROY:
		KillTimer(hwnd, TIMER_ID);
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
		hdc = GetDC(hwnd);

		int x = random_dword() % WIDTH;
		int y = random_dword() % HEIGHT;
		DWORD r = random_dword() % 256;
		DWORD g = random_dword() % 256;
		DWORD b = random_dword() % 256;

		SetPixel(hdc, x, y, RGB(r, g, b));

		ReleaseDC(hwnd, hdc);
		return 0;
	}
	return DefWindowProc(hwnd , msg , wp , lp);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	int nCmdShow
) {
	HWND hwnd;
	MSG msg;
	WNDCLASSW winc;

	winc.style			= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc;
	winc.cbClsExtra		= winc.cbWndExtra = 0;
	winc.hInstance		= hInstance;
	winc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= L"TestApp";

	if (!RegisterClassW(&winc)) return 0;

	hwnd = CreateWindowW(
		L"TestApp", L"Rand Vis",
		WS_OVERLAPPED | WS_SYSMENU,
		GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 4,
		512, 512, NULL, NULL,
		hInstance, NULL
	);

	if (hwnd == NULL) return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	my_randomize();

	SetTimer(hwnd, TIMER_ID, 10, NULL);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
