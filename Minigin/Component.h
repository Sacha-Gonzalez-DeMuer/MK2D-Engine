#pragma once
#include "Transform.h"
#include "IObject.h"
#include <memory>
#include "GameObject.h"

namespace dae
{
    class GameObject;

    class Component : public IObject
    {
    public:
        Component() {};

        void SetOwner(GameObject* owner) { m_owner = owner; };
    protected:
        GameObject* m_owner{ nullptr };

    private:
    };
}


