#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Stdafx.h"

class Node;

class Component
{
protected:
	inline Component() noexcept
		: _enabled(true)
		, _owner(nullptr)
	{
	}

	inline Component(const Component& component) noexcept = delete;
	inline Component(Component&& component) noexcept = delete;
	inline Component& operator=(const Component& component) noexcept = delete;
	inline Component& operator=(Component&& component) noexcept = delete;

public:
	virtual ~Component() noexcept = default;

public:
	inline void SetEnabled(bool enabled) noexcept
	{
		_enabled = enabled;
	}

	inline bool IsEnabled() const noexcept
	{
		return _enabled;
	}

	inline void SetOwner(Node* owner) noexcept
	{
		_owner = owner;
	}

	inline Node* GetOwner() const noexcept
	{
		return _owner;
	}

	virtual bool Init() = 0;
	virtual void PreUpdate(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void PostUpdate(float deltaTime) = 0;

protected:
	bool _enabled;
	Node* _owner;
};

#endif