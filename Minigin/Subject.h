#pragma once
#include "IObserver.h"
#include <vector>
#include <memory>
#include <functional>

namespace dae
{
    template<typename T> // i thought i was being clever with the templated subject&observer, but it turns out that that's very limiting :( need to redo
    class Subject
    {
    public:
        Subject() = default;
        virtual ~Subject() = default;

        void AddObserver(std::shared_ptr<IObserver<T>> observer)
        {
            m_Observers.push_back(observer);
        }

        void RemoveObserver(std::shared_ptr<IObserver<T>> observer)
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
        std::vector<std::shared_ptr<IObserver<T>>> m_Observers;
    };
}