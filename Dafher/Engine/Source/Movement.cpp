#include "Movement.h"
#include "Node.h"
#include "Transform.h"

void Movement::Update(float deltaTime)
{
	if (_acceleration != 0.0f)
	{
		_speed += _acceleration * deltaTime;

		if (_minSpeed != 0.0f || _maxSpeed != 0.0f)
		{
			if (_speed < _minSpeed)
			{
				_speed = _minSpeed;
			}

			if (_speed > _maxSpeed)
			{
				_speed = _maxSpeed;
			}
		}
	}

	if ((_velocity.x != 0.0f || _velocity.y != 0.0f || _velocity.z != 0.0f) && _speed != 0.0f)
	{
		Vector3 movement = _velocity * _speed * deltaTime;
		_owner->_transform->TranslateWorld(movement);
	}

	if ((_angularVelocity.x != 0.0f || _angularVelocity.y != 0.0f || _angularVelocity.z != 0.0f) && _angularSpeed != 0.0f)
	{
		if (_angularAcceleration != 0.0f)
		{
			_angularSpeed += _angularAcceleration * deltaTime;
		}

		if (_minAngularSpeed != 0.0f || _maxAngularSpeed != 0.0f)
		{
			if (_angularSpeed < _minAngularSpeed)
			{
				_angularSpeed = _minAngularSpeed;
			}
			if (_angularSpeed > _maxAngularSpeed)
			{
				_angularSpeed = _maxAngularSpeed;
			}
		}

		Vector3 rotation = _angularVelocity * _angularSpeed * deltaTime;
		_owner->_transform->RotateWorld(rotation);
	}
}

void Movement::LookAt(const Vector3& target, const Vector3& up) noexcept
{
	Vector3 direction = target - _owner->_transform->GetWorldPosition();

	float length = direction.Length();

	if (length < Transform::SMALLEST_TOLERANCE)
	{
		return;
	}

	direction /= length;

	Vector3 forward = direction;

	Vector3 right;
	right.x = forward.y * up.z - forward.z * up.y;
	right.y = forward.z * up.x - forward.x * up.z;
	right.z = forward.x * up.y - forward.y * up.x;

	float rightLength = right.Length();
	if (rightLength < Transform::SMALLEST_TOLERANCE)
	{
		Vector3 alternateUp = (std::abs(forward.y) < 0.99f) ? Vector3(0.0f, 0.0f, 1.0f) : Vector3(1.0f, 0.0f, 0.0f);
		right.x = forward.y * alternateUp.z - forward.z * alternateUp.y;
		right.y = forward.z * alternateUp.x - forward.x * alternateUp.z;
		right.z = forward.x * alternateUp.y - forward.y * alternateUp.x;
	}
	right /= right.Length();

	Vector3 newUp;
	newUp.x = right.y * forward.z - right.z * forward.y;
	newUp.y = right.z * forward.x - right.x * forward.z;
	newUp.z = right.x * forward.y - right.y * forward.x;

	Matrix rotationMatrix;
	rotationMatrix._11 = right.x;
	rotationMatrix._12 = right.y;
	rotationMatrix._13 = right.z;
	rotationMatrix._14 = 0.0f;

	rotationMatrix._21 = forward.x;
	rotationMatrix._22 = forward.y;
	rotationMatrix._23 = forward.z;
	rotationMatrix._24 = 0.0f;

	rotationMatrix._31 = newUp.x;
	rotationMatrix._32 = newUp.y;
	rotationMatrix._33 = newUp.z;
	rotationMatrix._34 = 0.0f;

	rotationMatrix._41 = 0.0f;
	rotationMatrix._42 = 0.0f;
	rotationMatrix._43 = 0.0f;
	rotationMatrix._44 = 1.0f;

	Quaternion targetRotation = Quaternion::CreateFromRotationMatrix(rotationMatrix);
	_owner->_transform->SetWorldRotation(targetRotation);
}

void Movement::LookAt(float x, float y, float z, const Vector3& up) noexcept
{
	LookAt(Vector3(x, y, z), up);
}