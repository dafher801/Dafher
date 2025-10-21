#ifndef __MOVEMENT_COMPONENT_H__
#define __MOVEMENT_COMPONENT_H__

#include "Component.h"

class MovementComponent : public Component
{
protected:
	inline MovementComponent() noexcept
		: Component()
		, _speed(0.0f)
		, _velocity(Vector3::Zero)
	{
	}

	inline MovementComponent(const MovementComponent& movementComponent) noexcept = delete;
	inline MovementComponent(MovementComponent&& movementComponent) noexcept = delete;
	inline MovementComponent& operator=(const MovementComponent& movementComponent) noexcept = delete;
	inline MovementComponent& operator=(MovementComponent&& movementComponent) noexcept = delete;

public:
	CREATE(MovementComponent)

	virtual ~MovementComponent() noexcept = default;

public:
	virtual bool Init() override;
	virtual void PreUpdate(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;

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
		_velocity = velocity;
	}

	inline void SetVelocity(float x, float y, float z = 0.0f) noexcept
	{
		_velocity = Vector3(x, y, z);
	}

	inline const Vector3& GetVelocity() const noexcept
	{
		return _velocity;
	}

private:
	float _speed;
	Vector3 _velocity;
};

#endif