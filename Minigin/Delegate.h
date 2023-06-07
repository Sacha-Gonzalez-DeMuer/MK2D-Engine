#pragma once
#include <functional>
#include <vector>

namespace dae
{
	template<typename ...Args>
	class Delegate
	{
	public:
		using MiniginDelegate = std::function<void(Args...)>;
		Delegate() = default;
		~Delegate() = default;
		Delegate(const Delegate& other) = delete;
		Delegate(Delegate&& other) noexcept = delete;
		Delegate& operator=(const Delegate& other) = delete;
		Delegate& operator=(Delegate&& other) noexcept = delete;
		void AddFunction(std::function<void(Args...)> function)
		{
			m_Functions.push_back(function);
		}
		void Invoke(Args... args)
		{
			for (std::function<void(Args...)>& function : m_Functions)
			{
				function(args...);
			}
		}

		void Clear()
		{
			m_Functions.clear();
		}

	private:
		std::vector<MiniginDelegate> m_Functions;
	};
}

