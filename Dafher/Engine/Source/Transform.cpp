#include "Transform.h"
#include "Node.h"

void Transform::SetLocalPosition(const Vector3& position) noexcept
{
    _position = position;
    MarkDirty();
}

void Transform::SetLocalPosition(float x, float y, float z) noexcept
{
    _position = Vector3(x, y, z);
    MarkDirty();
}

void Transform::SetLocalPositionX(float x) noexcept
{
    _position.x = x;
    MarkDirty();
}

void Transform::SetLocalPositionY(float y) noexcept
{
    _position.y = y;
    MarkDirty();
}

void Transform::SetLocalPositionZ(float z) noexcept
{
    _position.z = z;
    MarkDirty();
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

void Transform::SetLocalRotation(const Vector3& eulerAngles) noexcept
{
    _rotation = EulerToQuaternion(eulerAngles);
    MarkDirty();
}

void Transform::SetLocalRotation(float x, float y, float z) noexcept
{
    _rotation = EulerToQuaternion(Vector3(x, y, z));
    MarkDirty();
}

void Transform::SetLocalRotationX(float x) noexcept
{
    _rotation.x = x;
    MarkDirty();
}

void Transform::SetLocalRotationY(float y) noexcept
{
    _rotation.y = y;
    MarkDirty();
}

void Transform::SetLocalRotationZ(float z) noexcept
{
    _rotation.z = z;
    MarkDirty();
}

void Transform::SetWorldRotation(const Quaternion& quaternion) noexcept
{
    Matrix parentWorld = GetParentWorldMatrix();
    Quaternion parentQuaternion = ExtractRotation(parentWorld);
    Quaternion inverseQuaternion = parentQuaternion;
    inverseQuaternion.Inverse(inverseQuaternion);

    _rotation = inverseQuaternion * quaternion;
    MarkDirty();
}

void Transform::SetWorldRotation(const Vector3& eulerAngles) noexcept
{
    SetWorldRotation(EulerToQuaternion(eulerAngles));
}

void Transform::SetWorldRotation(float x, float y, float z) noexcept
{
    SetWorldRotation(Vector3(x, y, z));
}

Vector3 Transform::GetWorldRotationEuler() const noexcept
{
    return QuaternionToEuler(GetWorldRotationQuaternion());
}

Quaternion Transform::GetWorldRotationQuaternion() const noexcept
{
    Matrix parentWorldMatrix = GetParentWorldMatrix();
    Quaternion parentQuaternion = ExtractRotation(parentWorldMatrix);

    return parentQuaternion * _rotation;
}

void Transform::SetWorldRotationX(float x) noexcept
{
    Vector3 euler = GetWorldRotationEuler();
    euler.x = x;
    SetWorldRotation(euler);
}

void Transform::SetWorldRotationY(float y) noexcept
{
    Vector3 euler = GetWorldRotationEuler();
    euler.y = y;
    SetWorldRotation(euler);
}

void Transform::SetWorldRotationZ(float z) noexcept
{
    Vector3 euler = GetWorldRotationEuler();
    euler.z = z;
    SetWorldRotation(euler);
}

float Transform::GetWorldRotationX() const noexcept
{
    return GetWorldRotationEuler().x;
}

float Transform::GetWorldRotationY() const noexcept
{
    return GetWorldRotationEuler().y;
}

float Transform::GetWorldRotationZ() const noexcept
{
    return GetWorldRotationEuler().z;
}

void Transform::SetLocalScale(const Vector3& scale) noexcept
{
    _scale = scale;
    MarkDirty();
}

void Transform::SetLocalScale(float x, float y, float z) noexcept
{
    _scale = Vector3(x, y, z);
    MarkDirty();
}

void Transform::SetLocalScale(float uniformScale) noexcept
{
    _scale = Vector3(uniformScale, uniformScale, uniformScale);
    MarkDirty();
}

void Transform::SetLocalScaleX(float x) noexcept
{
    _scale.x = x;
    MarkDirty();
}

void Transform::SetLocalScaleY(float y) noexcept
{
    _scale.y = y;
    MarkDirty();
}

void Transform::SetLocalScaleZ(float z) noexcept
{
    _scale.z = z;
    MarkDirty();
}

void Transform::SetWorldScale(const Vector3& scale) noexcept
{
    Vector3 parentScale = ExtractScale(GetParentWorldMatrix());

    _scale.x = (parentScale.x != 0.0f) ? scale.x / parentScale.x : scale.x;
    _scale.y = (parentScale.y != 0.0f) ? scale.y / parentScale.y : scale.y;
    _scale.z = (parentScale.z != 0.0f) ? scale.z / parentScale.z : scale.z;

    MarkDirty();
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
    return ExtractScale(GetWorldMatrix());
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

void Transform::TranslateLocal(const Vector3& offset) noexcept
{
    _position += offset;
    MarkDirty();
}

void Transform::TranslateLocal(float x, float y, float z) noexcept
{
    _position += Vector3(x, y, z);
    MarkDirty();
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

void Transform::RotateLocal(const Vector3& eulerAngles) noexcept
{
    Quaternion deltaQuat = EulerToQuaternion(eulerAngles);
    _rotation = _rotation * deltaQuat;
    _rotation.Normalize();
    MarkDirty();
}

void Transform::RotateLocal(float x, float y, float z) noexcept
{
    RotateLocal(Vector3(x, y, z));
}

void Transform::RotateWorld(const Vector3& eulerAngles) noexcept
{
    Quaternion worldQuaternion = GetWorldRotationQuaternion();
    Quaternion deltaQuaternion = EulerToQuaternion(eulerAngles);
    Quaternion newWorldQuaternion = worldQuaternion * deltaQuaternion;
    newWorldQuaternion.Normalize();

    SetWorldRotation(newWorldQuaternion);
}

void Transform::RotateWorld(float x, float y, float z) noexcept
{
    RotateWorld(Vector3(x, y, z));
}

void Transform::ScaleByLocal(const Vector3& factor) noexcept
{
    _scale.x *= factor.x;
    _scale.y *= factor.y;
    _scale.z *= factor.z;
    MarkDirty();
}

void Transform::ScaleByLocal(float factor) noexcept
{
    _scale.x *= factor;
    _scale.y *= factor;
    _scale.z *= factor;
    MarkDirty();
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

void Transform::Reset() noexcept
{
    _position = Vector3::Zero;
    _rotation = Quaternion::Identity;
    _scale = Vector3::One;
    MarkDirty();
}

Vector3 Transform::GetWorldForward() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return Vector3(worldMatrix.m[1][0], worldMatrix.m[1][1], worldMatrix.m[1][2]);
}

Vector3 Transform::GetWorldRight() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return Vector3(worldMatrix.m[0][0], worldMatrix.m[0][1], worldMatrix.m[0][2]);
}

Vector3 Transform::GetWorldUp() const noexcept
{
    const Matrix& worldMatrix = GetWorldMatrix();
    return Vector3(worldMatrix.m[2][0], worldMatrix.m[2][1], worldMatrix.m[2][2]);
}

void Transform::MarkWorldMatrixDirty() noexcept
{
    _isWorldDirty = true;

    for (const auto& child : _owner->GetChildren())
    {
        child->_transform->MarkWorldMatrixDirty();
    }
}

void Transform::UpdateLocalMatrix() const noexcept
{
    _localMatrix = Matrix::CreateScale(_scale) *
        Matrix::CreateFromQuaternion(_rotation) *
        Matrix::CreateTranslation(_position);
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
    return matrix.Translation();
}

Quaternion Transform::ExtractRotation(const Matrix& matrix) const noexcept
{
    Vector3 scale = ExtractScale(matrix);

    Vector3 axisX = Vector3(matrix._11, matrix._12, matrix._13);
    Vector3 axisY = Vector3(matrix._21, matrix._22, matrix._23);
    Vector3 axisZ = Vector3(matrix._31, matrix._32, matrix._33);

    axisX = scale.x > SMALLEST_TOLERANCE ? axisX / scale.x : Vector3(1.0f, 0.0f, 0.0f);
    axisY = scale.y > SMALLEST_TOLERANCE ? axisY / scale.y : Vector3(0.0f, 1.0f, 0.0f);
    axisZ = scale.z > SMALLEST_TOLERANCE ? axisZ / scale.z : Vector3(0.0f, 0.0f, 1.0f);

    Matrix rotationMatrix;
    rotationMatrix._11 = axisX.x;
    rotationMatrix._12 = axisX.y;
    rotationMatrix._13 = axisX.z;

    rotationMatrix._21 = axisY.x;
    rotationMatrix._22 = axisY.y;
    rotationMatrix._23 = axisY.z;

    rotationMatrix._31 = axisZ.x;
    rotationMatrix._32 = axisZ.y;
    rotationMatrix._33 = axisZ.z;

    rotationMatrix._44 = 1.0f;
    rotationMatrix._41 = rotationMatrix._42 = rotationMatrix._43 = 0.0f;
    rotationMatrix._14 = rotationMatrix._24 = rotationMatrix._34 = 0.0f;

    return Quaternion::CreateFromRotationMatrix(rotationMatrix);
}

Vector3 Transform::ExtractScale(const Matrix& matrix) const noexcept
{
    Vector3 scaleX(matrix._11, matrix._12, matrix._13);
    Vector3 scaleY(matrix._21, matrix._22, matrix._23);
    Vector3 scaleZ(matrix._31, matrix._32, matrix._33);

    return Vector3(
        scaleX.Length(),
        scaleY.Length(),
        scaleZ.Length()
    );
}

Quaternion Transform::EulerToQuaternion(const Vector3& euler) noexcept
{
    return Quaternion::CreateFromYawPitchRoll(
        DirectX::XMConvertToRadians(euler.y),
        DirectX::XMConvertToRadians(euler.x),
        DirectX::XMConvertToRadians(euler.z)
    );
}

Vector3 Transform::QuaternionToEuler(const Quaternion& quaternion) noexcept
{
    float x = quaternion.x;
    float y = quaternion.y;
    float z = quaternion.z;
    float w = quaternion.w;

    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    float roll = std::atan2(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (w * y - z * x);
    float pitch;
    if (std::abs(sinp) >= 1.0f)
    {
        pitch = std::copysign(DirectX::XM_PI / 2.0f, sinp);
    }
    else
    {
        pitch = std::asin(sinp);
    }

    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    float yaw = std::atan2(siny_cosp, cosy_cosp);

    return Vector3(
        DirectX::XMConvertToDegrees(pitch),
        DirectX::XMConvertToDegrees(yaw),
        DirectX::XMConvertToDegrees(roll)
    );
}