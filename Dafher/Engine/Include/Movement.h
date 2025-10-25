#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "Component.h"

class Movement : public Component
{
protected:
	inline Movement() noexcept
		: Component()
		, _velocity(Vector3::Zero)
		, _angularVelocity(Vector3::Zero)
		, _speed(0.0f)
		, _acceleration(0.0f)
		, _minSpeed(0.0f)
		, _maxSpeed(0.0f)
		, _angularSpeed(0.0f)
		, _angularAcceleration(0.0f)
		, _minAngularSpeed(0.0f)
		, _maxAngularSpeed(0.0f)
	{
	}

	Movement(const Movement& Movement) noexcept = delete;
	Movement(Movement&& Movement) noexcept = delete;
	Movement& operator=(const Movement& Movement) noexcept = delete;
	Movement& operator=(Movement&& Movement) noexcept = delete;

public:
	virtual ~Movement() noexcept override = default;

public:
	CREATE(Movement)

public:
	virtual void Update(float deltaTime) override;

public:
	void LookAt(const Vector3& target, const Vector3& up = Vector3(0.0f, 0.0f, 1.0f)) noexcept;
	void LookAt(float x, float y, float z, const Vector3& up = Vector3(0.0f, 0.0f, 1.0f)) noexcept;

public:
	inline void SetSpeed(float speed) noexcept
	{
		_speed = speed;
	}

	inline float GetSpeed() const noexcept
	{
		return _speed;
	}

	inline void SetVelocity(const Vector3& velocity) noexcept
	{
		float length = std::sqrt(velocity.x * velocity.x +
			velocity.y * velocity.y +
			velocity.z * velocity.z);

		_velocity = length > 0.0f ? velocity / length : Vector3::Zero;
	}

	inline void SetVelocity(float x, float y, float z = 0.0f) noexcept
	{
		SetVelocity(Vector3(x, y, z));
	}

	inline Vector3 GetVelocity() const noexcept
	{
		return _velocity;
	}

	inline Vector3 GetVelocityWithSpeed() const noexcept
	{
		return _velocity * _speed;
	}

	inline void SetAcceleration(float acceleration) noexcept
	{
		_acceleration = acceleration;
	}

	inline float GetAcceleration() const noexcept
	{
		return _acceleration;
	}

	inline void SetMinSpeed(float minSpeed) noexcept
	{
		_minSpeed = MAX(0.0f, minSpeed);
		if (_maxSpeed < _minSpeed)
		{
			_maxSpeed = minSpeed;
		}
	}

	inline float GetMinSpeed() const noexcept
	{
		return _minSpeed;
	}

	inline void SetMaxSpeed(float maxSpeed) noexcept
	{
		_maxSpeed = MAX(0.0f, maxSpeed);
		if (_minSpeed > _maxSpeed)
		{
			_minSpeed = maxSpeed;
		}
	}

	inline float GetMaxSpeed() const noexcept
	{
		return _maxSpeed;
	}

	inline void SetSpeedRange(float minSpeed, float maxSpeed) noexcept
	{
		_minSpeed = minSpeed;
		_maxSpeed = maxSpeed;
	}

	inline void SetAngularVelocity(const Vector3& angularVelocity) noexcept
	{
		float length = std::sqrt(angularVelocity.x * angularVelocity.x +
			angularVelocity.y * angularVelocity.y +
			angularVelocity.z * angularVelocity.z);

		_angularVelocity = length > 0.0f ? angularVelocity / length : Vector3::Zero;
	}

	inline void SetAngularVelocity(float x, float y, float z = 0.0f) noexcept
	{
		SetAngularVelocity(Vector3(x, y, z));
	}

	inline Vector3 GetAngularVelocity() const noexcept
	{
		return _angularVelocity;
	}

	inline Vector3 GetAngularVelocityWithSpeed() const noexcept
	{
		return _angularVelocity * _angularSpeed;
	}

	inline void SetAngularSpeed(float angularSpeed) noexcept
	{
		_angularSpeed = angularSpeed;
	}

	inline float GetAngularSpeed() const noexcept
	{
		return _angularSpeed;
	}

	inline void SetAngularAcceleration(float angularAcceleration) noexcept
	{
		_angularAcceleration = angularAcceleration;
	}

	inline float GetAngularAcceleration() const noexcept
	{
		return _angularAcceleration;
	}

	inline void SetMinAngularSpeed(float minAngularSpeed) noexcept
	{
		_minAngularSpeed = minAngularSpeed;
		if (_maxAngularSpeed < _minAngularSpeed)
		{
			_maxAngularSpeed = minAngularSpeed;
		}
	}

	inline float GetMinAngularSpeed() const noexcept
	{
		return _minAngularSpeed;
	}

	inline void SetMaxAngularSpeed(float maxAngularSpeed) noexcept
	{
		_maxAngularSpeed = maxAngularSpeed;
		if (_minAngularSpeed > _maxAngularSpeed)
		{
			_minAngularSpeed = maxAngularSpeed;
		}
	}

	inline float GetMaxAngularSpeed() const noexcept
	{
		return _maxAngularSpeed;
	}

	inline void SetAngularSpeedRange(float minAngularSpeed, float maxAngularSpeed) noexcept
	{
		_minAngularSpeed = minAngularSpeed;
		_maxAngularSpeed = maxAngularSpeed;
	}

private:
	Vector3 _velocity;
	Vector3 _angularVelocity;
	float _speed;
	float _acceleration;
	float _minSpeed;
	float _maxSpeed;
	float _angularSpeed;
	float _angularAcceleration;
	float _minAngularSpeed;
	float _maxAngularSpeed;
};

#endif