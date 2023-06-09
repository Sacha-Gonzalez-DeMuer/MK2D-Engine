#pragma once
#include <memory>

namespace dae
{
	class PacNPC;
	class PacGrid;
	class PacNPCArrivalHandler
	{
	public:
		PacNPCArrivalHandler(std::shared_ptr<PacNPC> pNPC)
			:m_pNPC{ pNPC } {};

		void operator()(int, std::shared_ptr<PacGrid>);

	private:
		std::shared_ptr<PacNPC> m_pNPC;
	};
}
