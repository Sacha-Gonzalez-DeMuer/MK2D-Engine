#pragma once
#include <functional>
#include <vector>
#include <memory>	
#include <algorithm>
#include <utility>


namespace dae
{
	template<typename ...Args>
	class Delegate
	{
		using SharedFunc = std::shared_ptr<std::function<void(Args...)>>;
		using ReferencedObjects = std::vector<std::weak_ptr<void>>;  /*  https://chat.openai.com/share/a6a127dd-762f-43b6-ad0d-fdc6bd8603f4   */
		using MiniginDelegate = std::pair<SharedFunc, ReferencedObjects>;

	public:
		Delegate() = default;
		~Delegate() = default;
		Delegate(const Delegate& other) = delete;
		Delegate(Delegate&& other) noexcept = delete;
		Delegate& operator=(const Delegate& other) = delete;
		Delegate& operator=(Delegate&& other) noexcept = delete;


		void AddFunction(std::function<void(Args...)> function)
		{
			m_Delegates.emplace_back(
				std::make_pair(
					std::make_shared<std::function<void(Args...)>> (std::move(function))
					, ReferencedObjects{}));
		}

		void AddFunction(std::function<void(Args...)> function, const ReferencedObjects& referencedObject)
		{
			m_Delegates.emplace_back(std::make_pair(std::make_shared<std::function<void(Args...)>>(std::move(function)), ReferencedObjects{ std::move(referencedObject)}));
		}

		void RemoveFunction(std::function<void(Args...)> function)
		{
			m_Delegates.erase(std::remove_if(m_Delegates.begin(), m_Delegates.end(),
				[&function](const MiniginDelegate& delegate)
				{
					return *(delegate.first) == function;
				}), m_Delegates.end());
		}


		void Invoke(Args... args)
		{
			for (auto it = m_Delegates.begin(); it != m_Delegates.end();)
			{
				const auto& shared_func = it->first;
				const auto& referencedObjects = it->second;

				// Make sure all referenced objects still exist
				bool allObjectsExist = std::all_of(referencedObjects.begin(), referencedObjects.end(),
					[](const std::weak_ptr<void>& wp) { return !wp.expired(); });

				if (shared_func && allObjectsExist)
				{
					(*shared_func)(args...);
					++it;
				}
				else
				{
					it = m_Delegates.erase(it);
				}
			}
		}


		void Clear()
		{
			m_Delegates.clear();
		}

	private:

		std::vector<MiniginDelegate> m_Delegates;
	};
}
