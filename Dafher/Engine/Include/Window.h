#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Stdafx.h"

class Window
{
public:
	Window(const Window& window) noexcept = delete;
	Window& operator=(const Window& window) noexcept = delete;
	Window(Window&& window) noexcept = delete;

	Window() noexcept;
	~Window() noexcept = default;

public:
	void Init() noexcept;

	HWND GetHwnd() const noexcept;

	static LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
	HWND hWnd;
};

#endif