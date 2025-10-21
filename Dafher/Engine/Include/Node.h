#ifndef __NODE_H__
#define __NODE_H__

#include "Transform.h"
#include "Component.h"

class Node
{
public:
    inline Node() noexcept
        : _parent(nullptr)
        , _visible(true)
		, _enabled(true)
    {
    }

    virtual ~Node() noexcept = default;

    inline Node(const Node& node) noexcept = delete;
    inline Node(Node&& node) noexcept = delete;
    inline Node& operator=(const Node& node) noexcept = delete;
    inline Node& operator=(Node&& node) noexcept = delete;

public:
    inline void AddChild(Node* child) noexcept
    {
		assert(child != nullptr);

        _children.push_back(std::shared_ptr<Node>(child));
        child->_parent = std::shared_ptr<Node>(this);
    }

    void RemoveChild(Node* child) noexcept;

    inline Node* GetParent() const noexcept
    {
        return _parent.get();
    }

    inline const std::vector<std::shared_ptr<Node>>& GetChildren() const noexcept
    {
        return _children;
    }

    Node* GetChildByName(const std::string& name) const noexcept;

    inline const Matrix& GetLocalMatrix() const noexcept
    {
        return _transform.GetMatrix();
    }

    Matrix GetWorldMatrix() const noexcept;

    inline void SetVisible(bool visible) noexcept
    {
        _visible = visible;
    }

    inline bool IsVisible() const noexcept
    {
        return _visible;
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

    virtual bool Init();
    virtual void Update(float delta);
    virtual void Render();
    virtual void Clear();

public:
    Transform _transform;

protected:
    std::shared_ptr<Node> _parent;
    std::vector<std::shared_ptr<Node>> _children;
	std::vector<std::shared_ptr<Component>> _components;

    bool _visible;
    bool _enabled;
    std::string _name;
};

#endif