#ifndef __NODE_H__
#define __NODE_H__

#include "Transform.h"

class Node
{
protected:
	inline Node() noexcept
		: _transform(nullptr)
		, _parent(nullptr)
		, _enabled(true)
	{
	}

	Node(const Node& node) noexcept = delete;
	Node(Node&& node) noexcept = delete;
	Node& operator=(const Node& node) noexcept = delete;
	Node& operator=(Node&& node) noexcept = delete;

public:
	virtual ~Node() noexcept = default;

public:
	CREATE(Node)

public:
	virtual bool Init();
	virtual void PreUpdate(float delta);
	virtual void Update(float delta);
	virtual void PostUpdate(float delta);
	virtual void Clear();

public:
	void AddChild(Node* child) noexcept;
	void RemoveChild(Node* child) noexcept;
	Node* GetChildByName(const std::string& name) const noexcept;

public:
	inline Node* GetParent() const noexcept
	{
		return _parent;
	}

	inline const std::vector<std::unique_ptr<Node>>& GetChildren() const noexcept
	{
		return _children;
	}

	inline void SetEnabled(bool enabled) noexcept
	{
		_enabled = enabled;
	}

	inline bool IsEnabled() const noexcept
	{
		return _enabled;
	}

	inline void SetName(const std::string& name) noexcept
	{
		_name = name;
	}

	inline const std::string& GetName() const noexcept
	{
		return _name;
	}

public:
	template<typename T>
	inline T* AddComponent() noexcept
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		auto component = T::Create();
		component->SetOwner(this);
		_components.push_back(std::unique_ptr<T>(component));
		return component;
	}

	template<typename T>
	inline T* GetComponent() const noexcept
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		for (const auto& component : _components)
		{
			T* castedComponent = dynamic_cast<T*>(component.get());

			if (castedComponent != nullptr)
			{
				return castedComponent;
			}
		}

		return nullptr;
	}

	template<typename T>
	inline void RemoveComponent() noexcept
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		auto it = std::remove_if(_components.begin(), _components.end(),
			[](const std::unique_ptr<Component>& component)
			{
				return dynamic_cast<T*>(component.get()) != nullptr;
			});

		_components.erase(it, _components.end());
	}

public:
	std::vector<std::unique_ptr<Node>> _children;
	std::vector<std::unique_ptr<Component>> _components;
	std::string _name;
	Transform* _transform;
	Node* _parent;
	bool _enabled;
};

#endif