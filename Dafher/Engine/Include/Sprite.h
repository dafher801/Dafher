#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Node.h"

class Sprite : public Node
{
public:
	inline Sprite() noexcept
		: Node()
		, _texture(nullptr)
		, _isPlaying(false)
		, _loop(true)
		, _currentFrameIndex(0)
		, _frameTimer(0.0f)
		, _color(Vector4::One)
		, _onAnimationComplete(nullptr)
	{
	}

	Sprite(const std::string& textureKey) noexcept;

	~Sprite() noexcept = default;

	Sprite(const Sprite& sprite) noexcept = delete;
	Sprite& operator=(const Sprite& sprite) noexcept = delete;
	Sprite(Sprite&& sprite) noexcept = delete;

public:
	virtual bool Init() override;
	virtual void Update(float delta) override;
	virtual void Render() override;

	inline void Play(bool loop = true) noexcept
	{
		if (_textures.empty())
		{
			return;
		}

		_loop = loop;
		_isPlaying = true;

		_texture = _textures[0];
	}

	inline void Stop() noexcept
	{
		_isPlaying = false;
		_currentFrameIndex = 0;
		_frameTimer = 0.0f;
	}

	inline void Pause() noexcept
	{
		_isPlaying = false;
	}

	inline void Resume() noexcept
	{
		_isPlaying = true;
	}

	inline bool IsPlaying() const
	{
		return _isPlaying;
	}
	
	inline size_t GetCurrentFrameIndex() const
	{
		return _currentFrameIndex;
	}
	
	inline size_t GetFrameCount() const
	{
		return _textures.size();
	}

	void AddFrame(const std::string& textureKey, float duration = 0.1f);

	void UpdateAnimation(float delta) noexcept;
	
	inline void SetOnAnimationComplete(std::function<void()> callback) noexcept
	{
		_onAnimationComplete = callback;
	}

private:
	class Texture* _texture;

	std::vector<class Texture*> _textures;
	std::vector<float> _frameDurations;

	bool _isPlaying;
	bool _loop;
	size_t _currentFrameIndex;
	float _frameTimer;

	Color _color;

	std::function<void()> _onAnimationComplete;
};

#endif