#pragma once


namespace dae
{
	template<typename T>
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void Notify(const T& argument) = 0;
	};
}


