#include "Window.h"

void Window::Init() noexcept
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Procedure;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"MinimalWindow";

    RegisterClassExW(&wcex);

    _hWnd = CreateWindowW(L"MinimalWindow", L"최소한의 창", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(_hWnd, SW_SHOW);
}

LRESULT CALLBACK Window::Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}