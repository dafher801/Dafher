#include "Engine.h"
#include "Window.h"
#include "GraphicDevice.h"

Engine::Engine() noexcept
	: window(make_shared<Window>())
	, graphicDevice(make_shared<GraphicDevice>())
{
}

Engine::~Engine() noexcept
{
	window.reset();
	graphicDevice.reset();
}

Engine& Engine::GetInstance() noexcept
{
	static Engine instance;
	return instance;
}

void Engine::Init() noexcept
{
	window->Init();
	graphicDevice->Init();
}

void Engine::Update() noexcept
{
	graphicDevice->Render();
}

void Engine::Clear() noexcept
{
	graphicDevice->Clear();
}

shared_ptr<class Window> Engine::GetWindow() const noexcept
{
	return window;
}
