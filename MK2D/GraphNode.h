#pragma once
#include <glm/glm.hpp>


namespace dae
{
    class GraphNode
    {
    public:
        GraphNode() : m_idx(-1), m_position(0, 0) {}
        explicit GraphNode(int idx, const glm::vec2& pos) : m_idx(idx), m_position(pos) {}

        virtual ~GraphNode() = default;

        int GetIndex() const { return m_idx; }
        void SetIndex(int idx) { m_idx = idx; }

        bool operator==(const GraphNode& other) const { return m_idx == other.m_idx; }

        const glm::vec2& GetLocalPosition() const { return m_position; }
        void SetPosition(const glm::vec2& pos) { m_position = pos; }

    protected:
        int m_idx;
        glm::vec2 m_position;
    };
}
