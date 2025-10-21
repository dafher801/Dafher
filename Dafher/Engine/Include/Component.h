#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class Component
{
public:
	inline Component() noexcept
		: _enabled(true)
	{
	}

	virtual ~Component() noexcept = default;

	inline Component(const Component& component) noexcept = delete;
	inline Component(Component&& component) noexcept = delete;
	inline Component& operator=(const Component& component) noexcept = delete;
	inline Component& operator=(Component&& component) noexcept = delete;

public:
	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;

protected:
	bool _enabled;
};

#endif