#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Stdafx.h"

class Window
{
public:
	inline Window() noexcept
		: _hWnd(nullptr)
	{
	}

	Window(const Window& window) noexcept = delete;
	Window(Window&& window) noexcept = delete;
	Window& operator=(const Window& window) noexcept = delete;
	Window& operator=(Window&& window) noexcept = delete;

public:
	~Window() noexcept = default;

public:
	void Init() noexcept;

public:
	inline HWND GetHWnd() const noexcept
	{
		return _hWnd;
	}

private:
	static LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
	HWND _hWnd;
};

#endif