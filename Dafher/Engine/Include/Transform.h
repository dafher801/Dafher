#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"

class Transform : public Component
{
protected:
    Transform() noexcept
        : _position(Vector3::Zero)
        , _rotation(Quaternion::Identity)
        , _scale(Vector3::One)
        , _localMatrix(DirectX::XMMatrixIdentity())
        , _worldMatrix(DirectX::XMMatrixIdentity())
        , _isLocalDirty(true)
        , _isWorldDirty(true)
    {
    }

    Transform(const Transform& transform) noexcept = delete;
    Transform(Transform&& transform) noexcept = delete;
    Transform& operator=(const Transform& transform) noexcept = delete;
    Transform& operator=(Transform&& transform) noexcept = delete;

public:
    CREATE(Transform)

        ~Transform() noexcept = default;

public:
    inline void SetLocalPosition(const Vector3& position) noexcept
    {
        _position = position;
        MarkDirty();
    }

    inline void SetLocalPosition(float x, float y, float z = 0.0f) noexcept
    {
        _position = Vector3(x, y, z);
        MarkDirty();
    }

    inline Vector3 GetLocalPosition() const noexcept
    {
        return _position;
    }

    inline void SetLocalPositionX(float x) noexcept
    {
        _position.x = x;
        MarkDirty();
    }

    inline void SetLocalPositionY(float y) noexcept
    {
        _position.y = y;
        MarkDirty();
    }

    inline void SetLocalPositionZ(float z) noexcept
    {
        _position.z = z;
        MarkDirty();
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

    void SetWorldPosition(const Vector3& position) noexcept;
    void SetWorldPosition(float x, float y, float z = 0.0f) noexcept;
    Vector3 GetWorldPosition() const noexcept;

    void SetWorldPositionX(float x) noexcept;
    void SetWorldPositionY(float y) noexcept;
    void SetWorldPositionZ(float z) noexcept;

    float GetWorldPositionX() const noexcept;
    float GetWorldPositionY() const noexcept;
    float GetWorldPositionZ() const noexcept;

    inline void SetLocalRotation(const Vector3& eulerAngles) noexcept
    {
        _rotation = EulerToQuaternion(eulerAngles);
        MarkDirty();
    }

    inline void SetLocalRotation(float x, float y, float z = 0.0f) noexcept
    {
        _rotation = EulerToQuaternion(Vector3(x, y, z));
        MarkDirty();
    }

    inline Vector3 GetLocalRotationEuler() const noexcept
    {
        return QuaternionToEuler(_rotation);
    }

    inline void SetLocalRotationX(float x) noexcept
    {
        _rotation.x = x;
        MarkDirty();
    }

    inline void SetLocalRotationY(float y) noexcept
    {
        _rotation.y = y;
        MarkDirty();
    }

    inline void SetLocalRotationZ(float z) noexcept
    {
        _rotation.z = z;
        MarkDirty();
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

    inline void SetLocalScale(const Vector3& scale) noexcept
    {
        _scale = scale;
        MarkDirty();
    }

    inline void SetLocalScale(float x, float y, float z = 1.0f) noexcept
    {
        _scale = Vector3(x, y, z);
        MarkDirty();
    }

    inline void SetLocalScale(float uniformScale) noexcept
    {
        _scale = Vector3(uniformScale, uniformScale, uniformScale);
        MarkDirty();
    }

    inline const Vector3& GetLocalScale() const noexcept
    {
        return _scale;
    }

    inline void SetLocalScaleX(float x) noexcept
    {
        _scale.x = x;
        MarkDirty();
    }

    inline void SetLocalScaleY(float y) noexcept
    {
        _scale.y = y;
        MarkDirty();
    }

    inline void SetLocalScaleZ(float z) noexcept
    {
        _scale.z = z;
        MarkDirty();
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

    inline void TranslateLocal(const Vector3& offset) noexcept
    {
        _position += offset;
        MarkDirty();
    }

    inline void TranslateLocal(float x, float y, float z = 0.0f) noexcept
    {
        _position += Vector3(x, y, z);
        MarkDirty();
    }

    void TranslateWorld(const Vector3& offset) noexcept;
    void TranslateWorld(float x, float y, float z = 0.0f) noexcept;

    void RotateLocal(const Vector3& eulerAngles) noexcept;
    void RotateLocal(float x, float y, float z = 0.0f) noexcept;

    void RotateWorld(const Vector3& angles) noexcept;
    void RotateWorld(float x, float y, float z = 0.0f) noexcept;

    inline void ScaleByLocal(const Vector3& factor) noexcept
    {
        _scale.x *= factor.x;
        _scale.y *= factor.y;
        _scale.z *= factor.z;
        MarkDirty();
    }

    inline void ScaleByLocal(float factor) noexcept
    {
        _scale.x *= factor;
        _scale.y *= factor;
        _scale.z *= factor;
        MarkDirty();
    }

    void ScaleByWorld(const Vector3& factor) noexcept;
    void ScaleByWorld(float factor) noexcept;

    const Matrix& GetLocalMatrix() const noexcept;
    const Matrix& GetWorldMatrix() const noexcept;

    void Reset() noexcept;

    inline Vector3 GetLocalForward() const noexcept
    {
        return Vector3::Forward;
    }

    inline Vector3 GetLocalRight() const noexcept
    {
        float angleRad = DirectX::XMConvertToRadians(_rotation.z);
        return Vector3(std::cos(angleRad), std::sin(angleRad), 0.0f);
    }

    inline Vector3 GetLocalUp() const noexcept
    {
        float angleRad = DirectX::XMConvertToRadians(_rotation.z);
        return Vector3(-std::sin(angleRad), std::cos(angleRad), 0.0f);
    }

    Vector3 GetWorldForward() const noexcept;
    Vector3 GetWorldRight() const noexcept;
    Vector3 GetWorldUp() const noexcept;

    void MarkWorldMatrixDirty() noexcept;

public:
    virtual bool Init() override;
    virtual void PreUpdate(float deltaTime) override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;

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

private:
    Vector3 _position;
    Quaternion _rotation;
    Vector3 _scale;

    mutable Matrix _localMatrix;
    mutable Matrix _worldMatrix;
    mutable bool _isLocalDirty;
    mutable bool _isWorldDirty;

	constexpr static float TOLERANCE = 1e-8f;
};

#endif