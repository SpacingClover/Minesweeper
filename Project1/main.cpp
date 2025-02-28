#ifndef UNICODE
#define UNICODE
#endif 
#define WM_LBUTTONDOWN    0x0201

#include <WindowsX.h>
#include "minesweeper.h"

// globals were needed due to Win32 WINMSG environment, and to prioritize saving values rather than calculating them on demand
MineSweeper* minesweeper;
int winwidth,winheight,mousex,mousey;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    // Initialize graphics library
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiPlusToken;
    Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, nullptr);

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Default windows window",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete minesweeper;
    Gdiplus::GdiplusShutdown(gdiPlusToken);
    return 0;
}

void OnSize(HWND hwnd, UINT wParam, int width, int height);

void drawWindow(HDC hdc);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        drawWindow(hdc);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_SIZE:
    {
        int width = LOWORD(lParam);  // Macro to get the low-order word.
        int height = HIWORD(lParam); // Macro to get the high-order word.

        // Respond to the message:
        OnSize(hwnd, (UINT)wParam, width, height);
        break;
    }
    case WM_ACTIVATE:
    {
        minesweeper = new MineSweeper;
        minesweeper->initializeGrid();
        break;
    }
    case WM_LBUTTONUP:
    {
        minesweeper->lClickInput(mousex, mousey);
        InvalidateRect(NULL, NULL, NULL);
        break;
    }
    case WM_RBUTTONUP:
    {
        minesweeper->rClickInput(mousex, mousey);
        InvalidateRect(NULL, NULL, NULL);
        break;
    }
    case WM_MOUSEMOVE:
    {
        mousex = GET_X_LPARAM(lParam);
        mousey = GET_Y_LPARAM(lParam);
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnSize(HWND hwnd, UINT wParam, int width, int height) {
    winwidth = width;
    winheight = height;
}

void drawWindow(HDC hdc) {
    Gdiplus::Graphics gf(hdc);
    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 0));

    gf.FillRectangle(&brush, 0, 0, winwidth, winheight);
    minesweeper->draw(hdc,winwidth,winheight);
}