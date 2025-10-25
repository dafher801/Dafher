#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "Stdafx.h"

class Engine
{
private:
	Engine() noexcept;

	Engine(const Engine& engine) = delete;
	Engine(Engine&& engine) noexcept = delete;
	Engine& operator=(const Engine& engine) = delete;
	Engine& operator=(Engine&& engine) = delete;

public:
	~Engine() noexcept;

public:
	static Engine* GetInstance() noexcept;

public:
	void Init() noexcept;
	void PreUpdate() noexcept;
	void Update() noexcept;
	void PostUpdate() noexcept;
	void Clear() noexcept;

	void SetCurrentScene(class Scene* scene) noexcept;
	void ChangeScene(class Scene* scene) noexcept;

public:
	inline class GraphicDevice* GetDevice() const noexcept
	{
		return _graphicDevice.get();
	}

	inline class TextureManager* GetTextureManager() const noexcept
	{
		return _textureManager.get();
	}

	inline class Renderer* GetRenderer() const noexcept
	{
		return _renderer.get();
	}

	inline float GetDeltaTime() const noexcept
	{
		return _deltaTime;
	}

private:
	void CalculateDeltaTime() noexcept;

private:
	std::unique_ptr<class Window> _window;
	std::unique_ptr<class GraphicDevice> _graphicDevice;
	std::unique_ptr<class TextureManager> _textureManager;
	std::unique_ptr<class Renderer> _renderer;
	std::unique_ptr<class Scene> _currentScene;

	std::chrono::steady_clock::time_point _lastFrameTime;
	std::chrono::steady_clock::time_point _currentFrameTime;
	float _deltaTime;
};

#endif