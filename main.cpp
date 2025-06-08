#include <vector>
#include <Windows.h>

struct Point {
    int x;
    int y;
    int count;
};

std::vector<Point> points;
bool isDrawing = false;
int currentCount = 0, Count;

LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
) {
    HDC hdc;
	PAINTSTRUCT ps;

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        isDrawing = true;
        currentCount += 1;
        SetCapture(hwnd);
        return 0;
    case WM_LBUTTONUP:
        isDrawing = false;
        ReleaseCapture();
        return 0;
    case WM_MOUSEMOVE:
        if (isDrawing) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            points.push_back({ x, y, currentCount });
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    case WM_PAINT:
        int temp;
        hdc = BeginPaint(hwnd , &ps);
        //SetPixel(hdc, point.x, point.y, RGB(0xFF, 0, 0));
        if (!points.empty()) {
            MoveToEx(hdc, points[0].x, points[0].y, NULL);
            for (size_t i = 1; i < points.size(); ++i) {
                if (points[i].count == points[i - 1].count) {
                    LineTo(hdc, points[i].x, points[i].y);
                }
                else {
                    MoveToEx(hdc, points[i].x, points[i].y, NULL);
                }
            }
        }
        EndPaint(hwnd , &ps);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow
) {
    CONST WCHAR *szAppName = L"TestApp";
    WNDCLASSW wc;
    HWND hwnd;
    MSG msg;

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szAppName;

    if (!RegisterClassW(&wc)) return 0;

    hwnd = CreateWindowW(
        szAppName, L"",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
