#include "Node.h"
#include "Engine.h"
#include "GraphicDevice.h"

void Node::RemoveChild(Node* child) noexcept
{
    assert(child != nullptr);

    auto it = std::find_if(_children.begin(), _children.end(),
        [child](const std::unique_ptr<Node>& node)
        {
            return node.get() == child;
        });

    if (it != _children.end())
    {
        (*it)->_parent = nullptr;
        _children.erase(it);
    }
}

Node* Node::GetChildByName(const std::string& name) const noexcept
{
    for (const auto& child : _children)
    {
        if (child->_name == name)
        {
            return child.get();
        }
    }

    return nullptr;
}

Matrix Node::GetWorldMatrix() const noexcept
{
    assert(_parent != nullptr);

    Matrix worldMatrix = _transform.GetMatrix();
    worldMatrix = worldMatrix * _parent->GetWorldMatrix();

    return worldMatrix;
}

bool Node::Init()
{
    for (auto& component : _components)
    {
        component->Init();
	}

    for (auto& child : _children)
    {
        child->Init();
    }

    return true;
}

void Node::PreUpdate(float delta)
{
    for (auto& component : _components)
    {
        if (component->IsEnabled())
        {
            component->PreUpdate(delta);
        }
	}

    for (auto& child : _children)
    {
        if (child->IsEnabled())
        {
            child->PreUpdate(delta);
        }
    }
}

void Node::Update(float delta)
{
    for (auto& component : _components)
    {
        if (component->IsEnabled())
        {
            component->Update(delta);
        }
    }

    for (auto& child : _children)
    {
        if (child->IsEnabled())
        {
            child->Update(delta);
        }
    }
}

void Node::PostUpdate(float delta)
{
    for (auto& component : _components)
    {
        if (component->IsEnabled())
        {
            component->PostUpdate(delta);
        }
    }

    for (auto& child : _children)
    {
        if (child->IsEnabled())
        {
            child->PostUpdate(delta);
        }
    }
}

void Node::Clear()
{
	_components.clear();
    _children.clear();
}