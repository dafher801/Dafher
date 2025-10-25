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

	Component(const Component& component) noexcept = delete;
	Component(Component&& component) noexcept = delete;
	Component& operator=(const Component& component) noexcept = delete;
	Component& operator=(Component&& component) noexcept = delete;

public:
	virtual ~Component() noexcept = default;

public:
	virtual bool Init() { return true; }
	virtual void PreUpdate(float deltaTime) {}
	virtual void Update(float deltaTime) {}
	virtual void PostUpdate(float deltaTime) {}

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

protected:
	Node* _owner;
	bool _enabled;
};

#endif