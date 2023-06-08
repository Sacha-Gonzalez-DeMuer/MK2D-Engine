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

		template <typename Callable>
		void AddFunction(Callable&& function)
		{
			static_assert(std::is_invocable_v<Callable, Args...>, "Invalid function signature"); /* https://chat.openai.com/share/3c873eaa-baa5-4637-b04b-485ef5c551a5 */ 
			m_Functions.push_back(std::forward<Callable>(function));
		}

		void Invoke(Args... args)
		{
			for (std::function<void(Args...)>& function : m_Functions)
			{
				function(args...);
			}
		}

		/*void RemoveListener(const MiniginDelegate& callback) //jefs suggestion, need to look into it before implementing
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [&](const MiniginDelegate& cb) {
				return cb.target_type() == callback.target_type() && cb.target<void(*)(Args...)>() == callback.target<void(*)(Args...)>();
				});
			if (it != m_Listeners.end())
			{
				m_Listeners.erase(it);
			}
		}*/

		void Clear()
		{
			m_Functions.clear();
		}
	private:
		std::vector<MiniginDelegate> m_Functions;
	};
}
