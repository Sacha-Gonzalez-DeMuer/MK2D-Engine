#pragma once
#include "Component.h"
#include "Debug.h"
#include "CacheTrasher.h"

namespace W04_DataTypes
{
	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject
	{
	public:
		Transform t{};
		int ID{};

		GameObject& operator*=(int x)
		{
			ID *= x;
			return *this;
		}
	};

	class GameObjectAlt
	{
	public:
		Transform* t{};
		int ID{};

		GameObjectAlt& operator*=(int x)
		{
			ID *= x;
			return *this;
		}
	};
}

namespace dae
{
	class CacheTrashDebugger : public Component
	{
	public:
		CacheTrashDebugger() = default;

		virtual void Update() override
		{
			/*if (!m_inputReceived && InputManager::GetInstance().IsKeyDown(SDLK_SPACE))
			{
				m_inputReceived = true;
				PerformMeasurements();
			}*/
		}

		virtual void Render() const override
		{
			ImGui::Begin("CacheTrasher2000", nullptr, ImGuiWindowFlags_AlwaysAutoResize);	

			// Render the plot using ImGui
			if (!m_inputReceived)
			{
				ImGui::Text("Press SPACE to start the measurements");
			}
			else
			{
				// Render the plots using ImGui
				Debug::GetInstance().PlotData("Exercise 1", m_steps, m_avgTimes_1);
				Debug::GetInstance().PlotData("Exercise 2.1", m_steps, m_avgTimes_21);
				Debug::GetInstance().PlotData("Exercise 2.2", m_steps, m_avgTimes_22);
			}
			ImGui::End();
		}

	private:
		bool m_inputReceived{ false };
		std::vector<float> m_steps;
		std::vector<float> m_avgTimes_1;
		std::vector<float> m_avgTimes_21;
		std::vector<float> m_avgTimes_22;

		void PerformMeasurements()
		{
			constexpr int nrMeasurements{ 10 };
			constexpr int maxStep{ 1024 };
			constexpr int stepSize{ 2 };
			constexpr int nrObjects{ 10'000'000 };
			constexpr int multiplication{ 2 };

			auto cache_trasher{ std::make_unique<dae::CacheTrasher>() };

			// 1.0
			std::vector<int> int_cache(nrObjects);
			m_avgTimes_1 = cache_trasher->MeasureMultiplication<int>(int_cache, multiplication, nrMeasurements, stepSize, maxStep);


			// 2.1
			std::vector<W04_DataTypes::GameObject> transform_objects{ nrObjects };
			m_avgTimes_21 = cache_trasher->MeasureMultiplication<W04_DataTypes::GameObject>(transform_objects, multiplication, nrMeasurements, stepSize, maxStep);


			// 2.2
			std::vector<W04_DataTypes::GameObjectAlt> transform_pointer_objects{ nrObjects };
			m_avgTimes_22 = cache_trasher->MeasureMultiplication<W04_DataTypes::GameObjectAlt>(transform_pointer_objects, multiplication, nrMeasurements, stepSize, maxStep);
		}
	};
}

