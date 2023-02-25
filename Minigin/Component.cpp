#include "Component.h"
#include "GameObject.h"

const glm::vec3& dae::Component::GetPosition() const
{
    return m_gameObject.lock()->GetTransform().GetPosition();
};
