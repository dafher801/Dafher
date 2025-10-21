#include "Sprite.h"
#include "GraphicDevice.h"
#include "Engine.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Renderer.h"
#include "Node.h"

Sprite::Sprite(const std::string& textureKey) noexcept
    : Component()
    , _isPlaying(false)
    , _loop(true)
    , _currentFrameIndex(0)
    , _frameTimer(0.0f)
    , _color(Vector4::One)
    , _onAnimationComplete(nullptr)
{
    _texture = Engine::GetInstance()->GetTextureManager()->GetTexture(textureKey);
}

bool Sprite::Init()
{
    return true;
}

void Sprite::PreUpdate(float delta)
{
}

void Sprite::Update(float delta)
{
	UpdateAnimation(delta);
}

void Sprite::PostUpdate(float delta)
{
    Engine::GetInstance()->GetRenderer()->Draw(_texture, _owner->_transform.GetMatrix());
}

void Sprite::AddFrame(const std::string& textureKey, float duration)
{
    auto texture = Engine::GetInstance()->GetTextureManager()->GetTexture(textureKey);
    _textures.push_back(texture);
    _frameDurations.push_back(duration);
}

void Sprite::UpdateAnimation(float delta) noexcept
{
    if (!IsPlaying() || _textures.empty())
    {
        return;
	}

    _frameTimer += delta;

    if (_frameTimer >= _frameDurations[_currentFrameIndex])
    {
        _frameTimer -= _frameDurations[_currentFrameIndex];
        _currentFrameIndex++;

        if (_currentFrameIndex >= _textures.size())
        {
            if (_loop)
            {
                _currentFrameIndex = 0;
            }
            else if (_onAnimationComplete != nullptr)
            {
                _onAnimationComplete();
            }
			else
            {
                _currentFrameIndex = _textures.size() - 1;
                return;
            }
        }

        _texture = _textures[_currentFrameIndex];
    }
}
