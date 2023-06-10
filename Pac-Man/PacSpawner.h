#pragma once
#include "Component.h"
#include <memory>
namespace dae
{
	class PacNPC;
	class PacLevel;
	class PacSpawner final : public Component
	{
	public:
		PacSpawner(std::shared_ptr<PacLevel> grid);
		~PacSpawner() = default;

		void Initialize();

		std::vector<std::shared_ptr<GameObject>> GetNPCs() const { return m_pNPCs; }

	private:
		std::vector<std::shared_ptr<GameObject>> m_pNPCs;
		std::shared_ptr<PacLevel> m_pLevel;

		std::shared_ptr<GameObject> InitializeGenericNPC();
		std::shared_ptr<GameObject> InitializeBlinky();
		std::shared_ptr<GameObject> InitializePinky();
		std::shared_ptr<GameObject> InitializeInky();
		std::shared_ptr<GameObject> InitializeClyde();

		float m_ghostSize;
		std::vector<int> m_spawnIndices;
	};


}
