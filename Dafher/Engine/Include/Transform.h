#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Stdafx.h"

class Transform
{
public:
    Transform() noexcept;
    ~Transform() noexcept = default;
    Transform(const Transform& transform) noexcept = delete;
    Transform& operator=(const Transform& transform) noexcept = delete;
    Transform(Transform&& transform) noexcept = delete;

    inline void SetPosition(const Vector3& position) noexcept
    {
        _position = position;
        _isDirty = true;
    }

    inline void SetPosition(float x, float y, float z = 0.0f) noexcept
    {
        _position = Vector3(x, y, z);
        _isDirty = true;
    }

    inline const Vector3& GetPosition() const noexcept
    {
        return _position;
    }

    inline void SetPositionX(float x) noexcept
    {
        _position.x = x;
        _isDirty = true;
    }

    inline void SetPositionY(float y) noexcept
    {
        _position.y = y;
        _isDirty = true;
    }

    inline void SetPositionZ(float z) noexcept
    {
        _position.z = z;
        _isDirty = true;
    }

    inline float GetPositionX() const noexcept
    {
        return _position.x;
    }

    inline float GetPositionY() const noexcept
    {
        return _position.y;
    }

    inline float GetPositionZ() const noexcept
    {
        return _position.z;
    }

    inline void SetRotation(const Vector3& rotation) noexcept
    {
        _rotation = rotation;
        _isDirty = true;
    }

    inline void SetRotation(float x, float y, float z = 0.0f) noexcept
    {
        _rotation = Vector3(x, y, z);
        _isDirty = true;
    }

    inline const Vector3& GetRotation() const noexcept
    {
        return _rotation;
    }

    inline void SetRotationX(float x) noexcept
    {
        _rotation.x = x;
        _isDirty = true;
    }

    inline void SetRotationY(float y) noexcept
    {
        _rotation.y = y;
        _isDirty = true;
    }

    inline void SetRotationZ(float z) noexcept
    {
        _rotation.z = z;
        _isDirty = true;
    }

    inline float GetRotationX() const noexcept
    {
        return _rotation.x;
    }

    inline float GetRotationY() const noexcept
    {
        return _rotation.y;
    }

    inline float GetRotationZ() const noexcept
    {
        return _rotation.z;
    }

    inline void SetScale(const Vector3& scale) noexcept
    {
        _scale = scale;
        _isDirty = true;
    }

    inline void SetScale(float x, float y, float z = 1.0f) noexcept
    {
        _scale = Vector3(x, y, z);
        _isDirty = true;
    }

    inline void SetScale(float uniformScale) noexcept
    {
        _scale = Vector3(uniformScale, uniformScale, uniformScale);
        _isDirty = true;
    }

    inline const Vector3& GetScale() const noexcept
    {
        return _scale;
    }

    inline void SetScaleX(float x) noexcept
    {
        _scale.x = x;
        _isDirty = true;
    }

    inline void SetScaleY(float y) noexcept
    {
        _scale.y = y;
        _isDirty = true;
    }

    inline void SetScaleZ(float z) noexcept
    {
        _scale.z = z;
        _isDirty = true;
    }

    inline float GetScaleX() const noexcept
    {
        return _scale.x;
    }

    inline float GetScaleY() const noexcept
    {
        return _scale.y;
    }

    inline float GetScaleZ() const noexcept
    {
        return _scale.z;
    }

    inline void Translate(const Vector3& offset) noexcept
    {
        _position += offset;
        _isDirty = true;
    }

    inline void Translate(float x, float y, float z = 0.0f) noexcept
    {
        _position += Vector3(x, y, z);
        _isDirty = true;
    }

    inline void Rotate(const Vector3& angles) noexcept
    {
        _rotation += angles;
        _isDirty = true;
    }

    inline void Rotate(float x, float y, float z = 0.0f) noexcept
    {
        _rotation += Vector3(x, y, z);
        _isDirty = true;
    }

    inline void RotateZ(float angle) noexcept
    {
        _rotation.z += angle;
        _isDirty = true;
    }

    inline void ScaleBy(const Vector3& factor) noexcept
    {
        _scale.x *= factor.x;
        _scale.y *= factor.y;
        _scale.z *= factor.z;
        _isDirty = true;
    }

    inline void ScaleBy(float factor) noexcept
    {
        _scale.x *= factor;
        _scale.y *= factor;
        _scale.z *= factor;
        _isDirty = true;
    }

    const Matrix& GetMatrix() const noexcept;

    void Reset() noexcept;

    inline Vector3 GetForward() const noexcept
    {
        return Vector3::Forward;
    }

    inline Vector3 GetRight() const noexcept
    {
        float angleRad = DirectX::XMConvertToRadians(_rotation.z);
        return Vector3(std::cos(angleRad), std::sin(angleRad), 0.0f);
    }

    inline Vector3 GetUp() const noexcept
    {
        float angleRad = DirectX::XMConvertToRadians(_rotation.z);
        return Vector3(-std::sin(angleRad), std::cos(angleRad), 0.0f);
    }

private:
    void UpdateMatrix() const noexcept;

private:
    Vector3 _position;
    Vector3 _rotation;
    Vector3 _scale;

    mutable Matrix _matrix;
    mutable bool _isDirty;
};

#endif