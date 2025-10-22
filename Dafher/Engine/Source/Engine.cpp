#include "Engine.h"
#include "Window.h"
#include "GraphicDevice.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Scene.h"

Engine::Engine() noexcept
	: _window(std::make_unique<Window>())
	, _graphicDevice(std::make_unique<GraphicDevice>())
	, _textureManager(std::make_unique<TextureManager>())
	, _renderer(std::make_unique<Renderer>())
	, _currentScene(nullptr)
	, _deltaTime(0.0f)
{

}

Engine::~Engine() noexcept
{
	_window.reset();
	_graphicDevice.reset();
}

Engine* Engine::GetInstance() noexcept
{
	static Engine* instance;

	if (instance == nullptr)
	{
		instance = new Engine();
	}

	return instance;
}

void Engine::SetCurrentScene(Scene* scene) noexcept
{
	assert(scene != nullptr);

	_currentScene.reset(scene);
	_currentScene->Init();
}

void Engine::ChangeScene(Scene* scene) noexcept
{
	assert(scene != nullptr);

	_currentScene->Clear();
	_currentScene.reset(scene);
	_currentScene->Init();
}

void Engine::Init() noexcept
{
	_window->Init();
	_graphicDevice->Init(_window->_hWnd);
	_textureManager->Init(GetDevice()->GetD11Device());
	_renderer->Init(GetDevice()->GetD11Device(), GetDevice()->GetContext());
	_currentScene = std::unique_ptr<Scene>(Scene::Create());

	_lastFrameTime = std::chrono::steady_clock::now();
	_currentFrameTime = _lastFrameTime;
}

void Engine::PreUpdate() noexcept
{
	CalculateDeltaTime();

	_currentScene->PreUpdate(_deltaTime);
}

void Engine::Update() noexcept
{
	_currentScene->Update(_deltaTime);
}

void Engine::PostUpdate() noexcept
{
	_graphicDevice->BeginFrame();

	_currentScene->PostUpdate(_deltaTime);

	_graphicDevice->EndFrame();
}

void Engine::Clear() noexcept
{
	_graphicDevice->Clear();
}

void Engine::CalculateDeltaTime() noexcept
{
	_currentFrameTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> timeSpan = _currentFrameTime - _lastFrameTime;
	_deltaTime = timeSpan.count();

	if (_deltaTime > 1.0f / 60.0f)
	{
		_deltaTime = 1.0f / 60.0f;
	}

	_lastFrameTime = _currentFrameTime;
}