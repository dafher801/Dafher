#include "Transform.h"
#include "Node.h"

const Matrix& Transform::GetLocalMatrix() const noexcept
{
    if (_isLocalDirty)
    {
        UpdateLocalMatrix();
        _isLocalDirty = false;
    }

    return _localMatrix;
}

const Matrix& Transform::GetWorldMatrix() const noexcept
{
    if (_isWorldDirty)
    {
        UpdateWorldMatrix();
        _isWorldDirty = false;
    }

    return _worldMatrix;
}

bool Transform::Init()
{
    return true;
}

void Transform::PreUpdate(float deltaTime)
{
}

void Transform::Update(float deltaTime)
{
}

void Transform::PostUpdate(float deltaTime)
{
}

void Transform::UpdateLocalMatrix() const noexcept
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

    _localMatrix = scaleMatrix * rotationMatrix * translationMatrix;
}

void Transform::UpdateWorldMatrix() const noexcept
{
    const Matrix& localMatrix = GetLocalMatrix();
    Matrix parentMatrix = GetParentWorldMatrix();

    _worldMatrix = localMatrix * parentMatrix;
}

void Transform::MarkDirty() noexcept
{
    _isLocalDirty = true;
    MarkWorldMatrixDirty();
}

void Transform::MarkWorldMatrixDirty() noexcept
{
    _isWorldDirty = true;

    for (const auto& child : _owner->GetChildren())
    {
        child->_transform->MarkWorldMatrixDirty();
    }
}

void Transform::Reset() noexcept
{
    _position = Vector3::Zero;
    _rotation = Vector3::Zero;
    _scale = Vector3::One;
    _isLocalDirty = true;
    _isWorldDirty = true;
}

Matrix Transform::GetParentWorldMatrix() const noexcept
{
    if (_owner != nullptr && _owner->GetParent() != nullptr)
    {
        Transform* parentTransform = _owner->GetParent()->_transform;
        if (parentTransform != nullptr)
        {
            return parentTransform->GetWorldMatrix();
        }
    }

    return DirectX::XMMatrixIdentity();
}

Vector3 Transform::ExtractPosition(const Matrix& matrix) const noexcept
{
    return Vector3(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
}

Vector3 Transform::ExtractRotation(const Matrix& matrix) const noexcept
{
    float pitch, yaw, roll;
    pitch = std::asin(-matrix.m[2][1]);

    if (std::cos(pitch) > 0.0001f)
    {
        yaw = std::atan2(matrix.m[2][0], matrix.m[2][2]);
        roll = std::atan2(matrix.m[0][1], matrix.m[1][1]);
    }
    else
    {
        yaw = std::atan2(-matrix.m[0][2], matrix.m[0][0]);
        roll = 0.0f;
    }

    return Vector3(
        DirectX::XMConvertToDegrees(pitch),
        DirectX::XMConvertToDegrees(yaw),
        DirectX::XMConvertToDegrees(roll)
    );
}

Vector3 Transform::ExtractScale(const Matrix& matrix) const noexcept
{
    Vector3 scale;

    scale.x = std::sqrt(matrix.m[0][0] * matrix.m[0][0] +
        matrix.m[0][1] * matrix.m[0][1] +
        matrix.m[0][2] * matrix.m[0][2]);

    scale.y = std::sqrt(matrix.m[1][0] * matrix.m[1][0] +
        matrix.m[1][1] * matrix.m[1][1] +
        matrix.m[1][2] * matrix.m[1][2]);

    scale.z = std::sqrt(matrix.m[2][0] * matrix.m[2][0] +
        matrix.m[2][1] * matrix.m[2][1] +
        matrix.m[2][2] * matrix.m[2][2]);

    return scale;
}

void Transform::SetWorldPosition(const Vector3& position) noexcept
{
    Matrix parentMatrix = GetParentWorldMatrix();
    Matrix invParentMatrix = DirectX::XMMatrixInverse(nullptr, parentMatrix);

    DirectX::XMVECTOR worldPos = DirectX::XMLoadFloat3(
        reinterpret_cast<const DirectX::XMFLOAT3*>(&position)
    );
    DirectX::XMVECTOR localPos = DirectX::XMVector3TransformCoord(worldPos, invParentMatrix);

    DirectX::XMStoreFloat3(
        reinterpret_cast<DirectX::XMFLOAT3*>(&_position),
        localPos
    );

    MarkDirty();
}

void Transform::SetWorldPosition(float x, float y, float z) noexcept
{
    SetWorldPosition(Vector3(x, y, z));
}

Vector3 Transform::GetWorldPosition() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return ExtractPosition(worldMatrix);
}

void Transform::SetWorldPositionX(float x) noexcept
{
    Vector3 worldPos = GetWorldPosition();
    worldPos.x = x;
    SetWorldPosition(worldPos);
}

void Transform::SetWorldPositionY(float y) noexcept
{
    Vector3 worldPos = GetWorldPosition();
    worldPos.y = y;
    SetWorldPosition(worldPos);
}

void Transform::SetWorldPositionZ(float z) noexcept
{
    Vector3 worldPos = GetWorldPosition();
    worldPos.z = z;
    SetWorldPosition(worldPos);
}

float Transform::GetWorldPositionX() const noexcept
{
    return GetWorldPosition().x;
}

float Transform::GetWorldPositionY() const noexcept
{
    return GetWorldPosition().y;
}

float Transform::GetWorldPositionZ() const noexcept
{
    return GetWorldPosition().z;
}

void Transform::SetWorldRotation(const Vector3& rotation) noexcept
{
    if (_owner == nullptr || _owner->GetParent() == nullptr)
    {
        SetLocalRotation(rotation);
        return;
    }

    Vector3 parentRotation = _owner->GetParent()->_transform->GetWorldRotation();
    Vector3 localRotation = rotation - parentRotation;
    SetLocalRotation(localRotation);
}

void Transform::SetWorldRotation(float x, float y, float z) noexcept
{
    SetWorldRotation(Vector3(x, y, z));
}

Vector3 Transform::GetWorldRotation() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return ExtractRotation(worldMatrix);
}

void Transform::SetWorldRotationX(float x) noexcept
{
    Vector3 worldRot = GetWorldRotation();
    worldRot.x = x;
    SetWorldRotation(worldRot);
}

void Transform::SetWorldRotationY(float y) noexcept
{
    Vector3 worldRot = GetWorldRotation();
    worldRot.y = y;
    SetWorldRotation(worldRot);
}

void Transform::SetWorldRotationZ(float z) noexcept
{
    Vector3 worldRot = GetWorldRotation();
    worldRot.z = z;
    SetWorldRotation(worldRot);
}

float Transform::GetWorldRotationX() const noexcept
{
    return GetWorldRotation().x;
}

float Transform::GetWorldRotationY() const noexcept
{
    return GetWorldRotation().y;
}

float Transform::GetWorldRotationZ() const noexcept
{
    return GetWorldRotation().z;
}

void Transform::SetWorldScale(const Vector3& scale) noexcept
{
    if (_owner == nullptr || _owner->GetParent() == nullptr)
    {
        SetLocalScale(scale);
        return;
    }

    Vector3 parentScale = _owner->GetParent()->_transform->GetWorldScale();
    Vector3 localScale;
    localScale.x = (parentScale.x != 0.0f) ? scale.x / parentScale.x : scale.x;
    localScale.y = (parentScale.y != 0.0f) ? scale.y / parentScale.y : scale.y;
    localScale.z = (parentScale.z != 0.0f) ? scale.z / parentScale.z : scale.z;
    SetLocalScale(localScale);
}

void Transform::SetWorldScale(float x, float y, float z) noexcept
{
    SetWorldScale(Vector3(x, y, z));
}

void Transform::SetWorldScale(float uniformScale) noexcept
{
    SetWorldScale(Vector3(uniformScale, uniformScale, uniformScale));
}

Vector3 Transform::GetWorldScale() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return ExtractScale(worldMatrix);
}

void Transform::SetWorldScaleX(float x) noexcept
{
    Vector3 worldScale = GetWorldScale();
    worldScale.x = x;
    SetWorldScale(worldScale);
}

void Transform::SetWorldScaleY(float y) noexcept
{
    Vector3 worldScale = GetWorldScale();
    worldScale.y = y;
    SetWorldScale(worldScale);
}

void Transform::SetWorldScaleZ(float z) noexcept
{
    Vector3 worldScale = GetWorldScale();
    worldScale.z = z;
    SetWorldScale(worldScale);
}

float Transform::GetWorldScaleX() const noexcept
{
    return GetWorldScale().x;
}

float Transform::GetWorldScaleY() const noexcept
{
    return GetWorldScale().y;
}

float Transform::GetWorldScaleZ() const noexcept
{
    return GetWorldScale().z;
}

void Transform::TranslateWorld(const Vector3& offset) noexcept
{
    Vector3 worldPos = GetWorldPosition();
    worldPos += offset;
    SetWorldPosition(worldPos);
}

void Transform::TranslateWorld(float x, float y, float z) noexcept
{
    TranslateWorld(Vector3(x, y, z));
}

void Transform::RotateWorld(const Vector3& angles) noexcept
{
    Vector3 worldRot = GetWorldRotation();
    worldRot += angles;
    SetWorldRotation(worldRot);
}

void Transform::RotateWorld(float x, float y, float z) noexcept
{
    RotateWorld(Vector3(x, y, z));
}

void Transform::RotateWorldZ(float angle) noexcept
{
    Vector3 worldRot = GetWorldRotation();
    worldRot.z += angle;
    SetWorldRotation(worldRot);
}

void Transform::ScaleByWorld(const Vector3& factor) noexcept
{
    Vector3 worldScale = GetWorldScale();
    worldScale.x *= factor.x;
    worldScale.y *= factor.y;
    worldScale.z *= factor.z;
    SetWorldScale(worldScale);
}

void Transform::ScaleByWorld(float factor) noexcept
{
    ScaleByWorld(Vector3(factor, factor, factor));
}

Vector3 Transform::GetWorldForward() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return Vector3(worldMatrix.m[2][0], worldMatrix.m[2][1], worldMatrix.m[2][2]);
}

Vector3 Transform::GetWorldRight() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return Vector3(worldMatrix.m[0][0], worldMatrix.m[0][1], worldMatrix.m[0][2]);
}

Vector3 Transform::GetWorldUp() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return Vector3(worldMatrix.m[1][0], worldMatrix.m[1][1], worldMatrix.m[1][2]);
}