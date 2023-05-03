#pragma once
#include <glm/glm.hpp>
//
///* MODIFIED IMPLEMENTATION OF THE ELITE FRAMEWORK GRAPHS */
//
namespace dae
{
	class GraphNode
	{
	public:
		GraphNode() : m_idx(-1) {}
		explicit GraphNode(int idx) : m_idx(idx) {}

		virtual ~GraphNode() = default;

		int GetIndex() const { return m_idx; }
		void SetIndex(int idx) { m_idx = idx; }

		bool operator==(const GraphNode& other) const { return m_idx == other.m_idx; }

	protected:
		int m_idx;
	};

	class GraphNode2D : public GraphNode
	{
	public:
		GraphNode2D() : GraphNode(), m_position(0, 0) {}
		explicit GraphNode2D(int idx, const glm::vec2& pos) : GraphNode(idx), m_position(pos) {}

		virtual ~GraphNode2D() = default;

		const glm::vec2& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec2& pos) { m_position = pos; }

	protected:
		glm::vec2 m_position{ 0,0 };
	};

}
