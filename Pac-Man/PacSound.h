#pragma once
#include "IObserver.h"
#include "PacController.h"
namespace dae
{
	class PacSound : public IObserver<PacController*, PacEvent>
	{
		public:
		PacSound();
		~PacSound() = default;

		//override onnotify
		void Notify(PacController* pSubject, PacEvent event) override;
		void OnSubjectDestroy() override;

	};
}


