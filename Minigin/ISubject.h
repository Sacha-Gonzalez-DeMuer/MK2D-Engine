#pragma once
#include "IObserver.h"

#include <vector>
#include <memory>
namespace dae
{
	template<typename T>
	class ISubject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;

		void AddObserver(IObserver<T>* observer)
		{
			m_Observers.push_back(observer);
		}

		void RemoveObserver(IObserver<T>* observer)
		{
			m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
		}

	protected:
		void NotifyObservers(const T& argument) const
		{
			for (const auto& observer : m_Observers)
			{
				observer->Notify(argument);
			}
		}

	private:
		std::vector<IObserver<T>*> m_Observers;
	};



}