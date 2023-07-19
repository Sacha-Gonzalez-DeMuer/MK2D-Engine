#pragma once
#include "Transform.h"
#include "IObject.h"
#include <memory>
#include "GameObject.h"
#include <stdexcept>

namespace dae
{
    class GameObject;

    class Component : public IObject
    {
    public:
        Component() {};

        void SetOwner(std::weak_ptr<GameObject> owner) { m_gameObject = owner; };

        GameObject* GetOwner() const {
            auto ownerPtr = m_gameObject.lock();
            if (!ownerPtr)
            {
                throw std::runtime_error("GameObject has been destroyed");
            }
            return ownerPtr.get();
        }

    protected:
        std::weak_ptr<GameObject> m_gameObject{ };

    private:
    };
}


