#include "Transform.h"

Transform::Transform() noexcept
    : _position(Vector3::Zero)
    , _rotation(Vector3::Zero)
    , _scale(Vector3::One)
    , _matrix(DirectX::XMMatrixIdentity())
    , _isDirty(true)
{
}

const Matrix& Transform::GetMatrix() const noexcept
{
    if (_isDirty)
    {
        UpdateMatrix();
        _isDirty = false;
    }

    return _matrix;
}

void Transform::UpdateMatrix() const noexcept
{
    Matrix scaleMatrix = DirectX::XMMatrixScaling(_scale.x, _scale.y, _scale.z);

    Matrix rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
        DirectX::XMConvertToRadians(_rotation.x),
        DirectX::XMConvertToRadians(_rotation.y),
        DirectX::XMConvertToRadians(_rotation.z)
    );

    Matrix translationMatrix = DirectX::XMMatrixTranslation(
        _position.x,
        _position.y,
        _position.z
    );

    _matrix = scaleMatrix * rotationMatrix * translationMatrix;
}

void Transform::Reset() noexcept
{
    _position = Vector3::Zero;
    _rotation = Vector3::Zero;
    _scale = Vector3::One;
    _isDirty = true;
}