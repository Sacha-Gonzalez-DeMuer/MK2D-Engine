#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Datatypes.h"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class GameObject;
	class MoveCommand : public Command
	{
	public:
		MoveCommand(std::shared_ptr<GameObject> object, const glm::vec2& dir, float speed) 
			: m_Object(object), m_Direction(dir), m_Speed(speed) {};

		virtual ~MoveCommand() = default;
		virtual void Execute() override;

	protected:
		std::shared_ptr<GameObject> m_Object;
		glm::vec2 m_Direction;
		float m_Speed;
	};

	// 2d move
	class Graph2DNavComponent;
	class GraphMoveCommand final : public MoveCommand
	{
	public:
		GraphMoveCommand(std::shared_ptr<Graph2DNavComponent> navigator, const glm::vec2& dir, float speed)
			: MoveCommand(nullptr, dir, speed) {};
		virtual ~GraphMoveCommand() = default;
		virtual void Execute() override;

	private:
		std::shared_ptr<Graph2DNavComponent> m_Navigator;
		Direction m_Direction;
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
