#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class GameObject;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(std::shared_ptr<GameObject> object, const glm::vec2& dir, float speed) 
			: m_Object(object), m_Direction(dir), m_Speed(speed) {};

		virtual ~MoveCommand() = default;
		virtual void Execute() override;

	private:
		std::shared_ptr<GameObject> m_Object;
		glm::vec2 m_Direction;
		float m_Speed;
	};


	class HealthComponent;
	class KillCommand final : public Command
	{
	public:
		KillCommand(std::shared_ptr<HealthComponent> health)
			: m_Health(health) {};
		virtual ~KillCommand() = default;
		virtual void Execute() override;

	private:
		std::shared_ptr<HealthComponent> m_Health;
	};
}
