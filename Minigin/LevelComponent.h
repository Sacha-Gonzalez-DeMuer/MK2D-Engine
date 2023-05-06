#pragma once
#include "Component.h"
#include <string>
namespace dae
{
	class RenderComponent;
	class GridGraph;
	class LevelComponent final : public Component
	{
	public:
		LevelComponent(const std::wstring& levelPath);
		~LevelComponent() = default;

		virtual void Render() const override;

		std::shared_ptr<GridGraph> GetGrid() const { return m_pGridGraph; }


	private:
		std::shared_ptr<GridGraph> m_pGridGraph;
	};
}


