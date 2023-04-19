#pragma once
#include <memory>

namespace dae
{
	template<typename T>
	class IObserver 
	{
	public:
		virtual ~IObserver() = default;
		virtual void Notify(const T& argument) = 0;
	};

	class BasicObserver {
	public:
		virtual ~BasicObserver() {}

		virtual void Notify() = 0;
	};
}


