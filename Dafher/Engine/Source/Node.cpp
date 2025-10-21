#include "Node.h"
#include "Engine.h"
#include "GraphicDevice.h"

void Node::RemoveChild(Node* child) noexcept
{
    assert(child != nullptr);

    auto it = std::find_if(_children.begin(), _children.end(),
        [child](const std::shared_ptr<Node>& node)
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
    Matrix worldMatrix = _transform.GetMatrix();

    if (_parent)
    {
        worldMatrix = worldMatrix * _parent->GetWorldMatrix();
    }

    return worldMatrix;
}

bool Node::Init()
{
    for (auto& child : _children)
    {
        child->Init();
    }

    return true;
}

void Node::Update(float delta)
{
    for (auto& child : _children)
    {
        if (child->IsEnabled())
        {
            child->Update(delta);
        }
    }
}

void Node::Render()
{
    for (auto& child : _children)
    {
        if (child->IsVisible() && child->IsEnabled())
        {
            child->Render();
        }
    }
}

void Node::Clear()
{
    for (auto& child : _children)
    {
        child->Clear();
    }
}