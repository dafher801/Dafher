#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"

class Transform : public Component
{
protected:
    inline Transform() noexcept
        : _localMatrix(DirectX::XMMatrixIdentity())
        , _worldMatrix(DirectX::XMMatrixIdentity())
        , _position(Vector3::Zero)
        , _scale(Vector3::One)
        , _rotation(Quaternion::Identity)
        , _isLocalDirty(true)
        , _isWorldDirty(true)
    {
    }

    Transform(const Transform& transform) noexcept = delete;
    Transform(Transform&& transform) noexcept = delete;
    Transform& operator=(const Transform& transform) noexcept = delete;
    Transform& operator=(Transform&& transform) noexcept = delete;

public:
    virtual ~Transform() noexcept override = default;

public:
    CREATE(Transform)

public:
    void SetLocalPosition(const Vector3& position) noexcept;
    void SetLocalPosition(float x, float y, float z = 0.0f) noexcept;
    void SetLocalPositionX(float x) noexcept;
    void SetLocalPositionY(float y) noexcept;
    void SetLocalPositionZ(float z) noexcept;

    void SetWorldPosition(const Vector3& position) noexcept;
    void SetWorldPosition(float x, float y, float z = 0.0f) noexcept;
    Vector3 GetWorldPosition() const noexcept;
    void SetWorldPositionX(float x) noexcept;
    void SetWorldPositionY(float y) noexcept;
    void SetWorldPositionZ(float z) noexcept;
    float GetWorldPositionX() const noexcept;
    float GetWorldPositionY() const noexcept;
    float GetWorldPositionZ() const noexcept;

    void SetLocalRotation(const Vector3& eulerAngles) noexcept;
    void SetLocalRotation(float x, float y, float z = 0.0f) noexcept;
    void SetLocalRotationX(float x) noexcept;
    void SetLocalRotationY(float y) noexcept;
    void SetLocalRotationZ(float z) noexcept;

    void SetWorldRotation(const Quaternion& rotation) noexcept;
    void SetWorldRotation(const Vector3& rotation) noexcept;
    void SetWorldRotation(float x, float y, float z = 0.0f) noexcept;
    Vector3 GetWorldRotationEuler() const noexcept;
    Quaternion GetWorldRotationQuaternion() const noexcept;
    void SetWorldRotationX(float x) noexcept;
    void SetWorldRotationY(float y) noexcept;
    void SetWorldRotationZ(float z) noexcept;
    float GetWorldRotationX() const noexcept;
    float GetWorldRotationY() const noexcept;
    float GetWorldRotationZ() const noexcept;

    void SetLocalScale(const Vector3& scale) noexcept;
    void SetLocalScale(float x, float y, float z = 1.0f) noexcept;
    void SetLocalScale(float uniformScale) noexcept;
    void SetLocalScaleX(float x) noexcept;
    void SetLocalScaleY(float y) noexcept;
    void SetLocalScaleZ(float z) noexcept;

    void SetWorldScale(const Vector3& scale) noexcept;
    void SetWorldScale(float x, float y, float z = 1.0f) noexcept;
    void SetWorldScale(float uniformScale) noexcept;
    Vector3 GetWorldScale() const noexcept;
    void SetWorldScaleX(float x) noexcept;
    void SetWorldScaleY(float y) noexcept;
    void SetWorldScaleZ(float z) noexcept;
    float GetWorldScaleX() const noexcept;
    float GetWorldScaleY() const noexcept;
    float GetWorldScaleZ() const noexcept;

    void TranslateLocal(const Vector3& offset) noexcept;
    void TranslateLocal(float x, float y, float z = 0.0f) noexcept;
    void TranslateWorld(const Vector3& offset) noexcept;
    void TranslateWorld(float x, float y, float z = 0.0f) noexcept;

    void RotateLocal(const Vector3& eulerAngles) noexcept;
    void RotateLocal(float x, float y, float z = 0.0f) noexcept;
    void RotateWorld(const Vector3& angles) noexcept;
    void RotateWorld(float x, float y, float z = 0.0f) noexcept;

    void ScaleByLocal(const Vector3& factor) noexcept;
    void ScaleByLocal(float factor) noexcept;
    void ScaleByWorld(const Vector3& factor) noexcept;
    void ScaleByWorld(float factor) noexcept;

    const Matrix& GetLocalMatrix() const noexcept;
    const Matrix& GetWorldMatrix() const noexcept;

    void Reset() noexcept;

    Vector3 GetWorldForward() const noexcept;
    Vector3 GetWorldRight() const noexcept;
    Vector3 GetWorldUp() const noexcept;

    void MarkWorldMatrixDirty() noexcept;

public:
    inline Vector3 GetLocalPosition() const noexcept
    {
        return _position;
    }

    inline float GetLocalPositionX() const noexcept
    {
        return _position.x;
    }

    inline float GetLocalPositionY() const noexcept
    {
        return _position.y;
    }

    inline float GetLocalPositionZ() const noexcept
    {
        return _position.z;
    }

    inline Vector3 GetLocalRotationEuler() const noexcept
    {
        return QuaternionToEuler(_rotation);
    }

    inline float GetLocalRotationX() const noexcept
    {
        return _rotation.x;
    }

    inline float GetLocalRotationY() const noexcept
    {
        return _rotation.y;
    }

    inline float GetLocalRotationZ() const noexcept
    {
        return _rotation.z;
    }

    inline Vector3 GetLocalScale() const noexcept
    {
        return _scale;
    }

    inline float GetLocalScaleX() const noexcept
    {
        return _scale.x;
    }

    inline float GetLocalScaleY() const noexcept
    {
        return _scale.y;
    }

    inline float GetLocalScaleZ() const noexcept
    {
        return _scale.z;
    }

    inline Vector3 GetLocalForward() const noexcept
    {
        return Vector3(0.0f, 1.0f, 0.0f);
    }

    inline Vector3 GetLocalRight() const noexcept
    {
        return Vector3(1.0f, 0.0f, 0.0f);
    }

    inline Vector3 GetLocalUp() const noexcept
    {
        return Vector3(0.0f, 0.0f, 1.0f);
    }

private:
    void UpdateLocalMatrix() const noexcept;
    void UpdateWorldMatrix() const noexcept;
    void MarkDirty() noexcept;

    Matrix GetParentWorldMatrix() const noexcept;
    Vector3 ExtractPosition(const Matrix& matrix) const noexcept;
    Quaternion ExtractRotation(const Matrix& matrix) const noexcept;
    Vector3 ExtractScale(const Matrix& matrix) const noexcept;

    static Quaternion EulerToQuaternion(const Vector3& euler) noexcept;
    static Vector3 QuaternionToEuler(const Quaternion& quaternion) noexcept;

public:
    constexpr static float SMALLEST_TOLERANCE = 1e-6f;
    constexpr static float ROTATION_TOLERANCE = 1.0f;
    constexpr static float PRECISE_ROTATION_TOLERANCE = 0.1f;
    constexpr static float LOOSE_ROTATION_TOLERANCE = 5.0f;

private:
    mutable Matrix _localMatrix;
    mutable Matrix _worldMatrix;

    Vector3 _position;
    Vector3 _scale;
    Quaternion _rotation;

    mutable bool _isLocalDirty;
    mutable bool _isWorldDirty;
};

#endif