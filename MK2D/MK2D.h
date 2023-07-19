#pragma once
#include <string>
#include <functional>

namespace dae
{
	class MK2D
	{
	public:
		explicit MK2D(const std::string& dataPath);
		~MK2D();
		void Run(const std::function<void()>& load);

		MK2D(const MK2D& other) = delete;
		MK2D(MK2D&& other) = delete;
		MK2D& operator=(const MK2D& other) = delete;
		MK2D& operator=(MK2D&& other) = delete;
	};
}