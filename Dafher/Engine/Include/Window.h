#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Stdafx.h"

class Window
{
public:
	friend class Engine;

	inline Window() noexcept
		: _hWnd(nullptr)
	{
	}

	~Window() noexcept = default;

	Window(const Window& window) noexcept = delete;
	Window& operator=(const Window& window) noexcept = delete;
	Window(Window&& window) noexcept = delete;

private:
	void Init() noexcept;

	static LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
	HWND _hWnd;
};

#endif