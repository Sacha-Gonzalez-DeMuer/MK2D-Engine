#pragma once
#include "Transform.h"
#include "IObject.h"
#include <memory>

namespace dae
{
    class GameObject;
    class Component : public IObject
    {
    public:
        explicit Component(std::weak_ptr<GameObject> owner) 
            : m_gameObject{ owner } {};

        const glm::vec3& GetPosition() const;
        std::weak_ptr<GameObject> GetOwner() { return m_gameObject; };

    protected:
        std::weak_ptr<GameObject> m_gameObject;
    };
}