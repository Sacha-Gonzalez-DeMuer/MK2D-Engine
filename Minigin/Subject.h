#pragma once
#include "IObserver.h"
#include <vector>
#include <memory>
#include <functional>

namespace dae
{
    template<typename... Args> // i thought i was being clever with the templated subject&observer, but it turns out that that's very limiting :( need to redo
    class Subject
    {
    public:
        Subject() = default;
        virtual ~Subject() = default;

        void AddObserver(std::shared_ptr<IObserver<Args...>> observer)
        {
            m_Observers.push_back(observer);
        }

        void RemoveObserver(std::shared_ptr<IObserver<Args...>> observer)
        {
            m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
        }

    protected:
        void NotifyObservers(Args... args) const
        {
            for (const auto& observer : m_Observers)
                observer->Notify(args...);
        }

    private:
        std::vector<std::shared_ptr<IObserver<Args...>>> m_Observers;
    };
}