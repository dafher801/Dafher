#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Component.h"
#include "Texture.h"

class Sprite : public Component
{
protected:
	inline Sprite() noexcept
		: Component()
		, _texture(nullptr)
		, _isPlaying(false)
		, _loop(true)
		, _isDirty(false)
		, _currentFrameIndex(0)
		, _frameTimer(0.0f)
		, _color(Vector4::One)
		, _size(Vector2::Zero)
		, _onAnimationComplete(nullptr)
	{
	}

	Sprite(const std::string& textureKey) noexcept;
	Sprite(const std::string& textureKey, uint32 width, uint32 height) noexcept;

	Sprite(const Sprite& sprite) noexcept = delete;
	Sprite(Sprite&& sprite) noexcept = delete;
	Sprite& operator=(const Sprite& sprite) noexcept = delete;
	Sprite& operator=(Sprite&& sprite) noexcept = delete;

public:
	CREATE(Sprite)

	~Sprite() noexcept = default;

public:
	inline void SetSize(uint32 width, uint32 height) noexcept
	{
		if (static_cast<uint32>(_size.x) == width &&
			static_cast<uint32>(_size.y) == height)
		{
			return;
		}

		_size.x = static_cast<float>(width);
		_size.y = static_cast<float>(height);
		_isDirty = true;
	}

	inline void SetWidth(uint32 width) noexcept
	{
		if (static_cast<uint32>(_size.x) == width)
		{
			return;
		}

		_size.x = static_cast<float>(width);
		_isDirty = true;
	}

	inline void SetHeight(uint32 height) noexcept
	{
		if (static_cast<uint32>(_size.y) == height)
		{
			return;
		}

		_size.y = static_cast<float>(height);
		_isDirty = true;
	}

	inline uint32 GetWidth() const noexcept
	{
		return static_cast<uint32>(_size.x);
	}

	inline uint32 GetHeight() const noexcept
	{
		return static_cast<uint32>(_size.y);
	}

	inline const Vector2& GetSize() const noexcept
	{
		return _size;
	}

	inline void Play(bool loop = true) noexcept
	{
		_loop = loop;
		_isPlaying = true;

		if (_textures.empty())
		{
			return;
		}
		
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

	inline void SetOnAnimationComplete(std::function<void()> callback) noexcept
	{
		_onAnimationComplete = callback;
	}

public:
	virtual bool Init() override;
	virtual void PreUpdate(float delta) override;
	virtual void Update(float delta) override;
	virtual void PostUpdate(float delta) override;

	void AddFrame(const std::string& textureKey, float duration = 0.1f);
	void UpdateAnimation(float delta) noexcept;

private:
	class Texture* _texture;

	std::vector<class Texture*> _textures;
	std::vector<float> _frameDurations;

	bool _isPlaying;
	bool _loop;
	bool _isDirty;
	float _frameTimer;
	uint32 _currentFrameIndex;

	Color _color;
	Vector2 _size;

	std::function<void()> _onAnimationComplete;
};

#endif