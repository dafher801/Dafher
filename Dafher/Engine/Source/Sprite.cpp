#include "Sprite.h"
#include "GraphicDevice.h"
#include "Engine.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Node.h"

Sprite::Sprite(const std::string& textureKey) noexcept
    : Component()
    , _isPlaying(false)
    , _loop(true)
    , _isDirty(false)
    , _currentFrameIndex(0)
    , _frameTimer(0.0f)
    , _color(Vector4::One)
    , _size(Vector2::Zero)
    , _anchorPoint(Vector2(0.5f, 0.5f))
    , _onAnimationComplete(nullptr)
{
    _texture = Engine::GetInstance()->GetTextureManager()->GetTexture(textureKey);
	_size.x = static_cast<float>(_texture->GetWidth());
	_size.y = static_cast<float>(_texture->GetHeight());
}

Sprite::Sprite(const std::string& textureKey, uint32 width, uint32 height) noexcept
    : Component()
    , _isPlaying(false)
    , _loop(true)
    , _isDirty(false)
    , _currentFrameIndex(0)
    , _frameTimer(0.0f)
    , _color(Vector4::One)
    , _size(Vector2(static_cast<float>(width), static_cast<float>(height)))
    , _anchorPoint(Vector2(0.5f, 0.5f))
    , _onAnimationComplete(nullptr)
{
    _texture = Engine::GetInstance()->GetTextureManager()->GetTexture(textureKey);
    _texture->Resize(static_cast<uint32>(_size.x), static_cast<uint32>(_size.y));
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
    if (_texture == nullptr)
    {
        return;
    }

    if (_isDirty)
    {
        _texture->Resize(static_cast<uint32>(_size.x), static_cast<uint32>(_size.y));
		_isDirty = false;
    }

    float offsetX = -_size.x * _anchorPoint.x;
    float offsetY = -_size.y * _anchorPoint.y;

    Matrix spriteScaleMatrix = DirectX::XMMatrixScaling(_size.x, _size.y, 1.0f);
    Matrix anchorOffsetMatrix = DirectX::XMMatrixTranslation(offsetX, offsetY, 0.0f);    
    Matrix worldMatrix = spriteScaleMatrix * anchorOffsetMatrix * _owner->_transform->GetWorldMatrix();

    Engine::GetInstance()->GetRenderer()->Draw(_texture, worldMatrix);
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
