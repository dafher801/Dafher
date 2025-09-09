#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "stdafx.h"

class Engine
{
public:
	Engine(const Engine& engine) = delete;
	Engine& operator=(const Engine& engine) = delete;
	Engine(Engine&& engine) noexcept = delete;
	
	~Engine() noexcept;

	static Engine& GetInstance() noexcept;

private:
	Engine() noexcept;
	
public:
	void Init() noexcept;
	void Update() noexcept;
	void Clear() noexcept;

	shared_ptr<class Window> GetWindow() const noexcept;

private:
	shared_ptr<class Window> window;
	shared_ptr<class GraphicDevice> graphicDevice;
};

#endif