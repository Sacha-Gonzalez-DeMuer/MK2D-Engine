#pragma once
#include <memory>

namespace dae
{
	template<typename... Args>
	class IObserver 
	{
	public:
		virtual ~IObserver() = default;
		virtual void Notify(Args... args) = 0;
		virtual void OnSubjectDestroy() = 0;
	};
}


