#include "MovementComponent.h"
#include "Node.h"

bool MovementComponent::Init()
{
	return true;
}

void MovementComponent::PreUpdate(float deltaTime)
{
}

void MovementComponent::Update(float deltaTime)
{
	assert(_owner != nullptr);

	if (_velocity.x != 0.0f || _velocity.y != 0.0f || _velocity.z != 0.0f)
	{
		float length = std::sqrt(_velocity.x * _velocity.x +
			_velocity.y * _velocity.y +
			_velocity.z * _velocity.z);

		if (length > 0.0f)
		{
			Vector3 normalized = _velocity;
			normalized /= length;

			Vector3 movement = normalized * _speed * deltaTime;
			_owner->_transform.Translate(movement);
		}
	}
}

void MovementComponent::PostUpdate(float deltaTime)
{
}