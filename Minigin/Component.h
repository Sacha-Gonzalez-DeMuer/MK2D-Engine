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
        Component() {};

        void AttachToGameObject(std::weak_ptr<GameObject> parent) { m_gameObject = parent; };
        const glm::vec3& GetPosition() const;

    protected:
        std::weak_ptr<GameObject> m_gameObject;

    private:
    };
}


