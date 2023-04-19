#pragma once
#include "Component.h"
#include "IObserver.h"

namespace dae
{
	class TextComponent;
	class ScoreComponent;
	class Font;

	class ScoreHUD final
		: public Component
		, public IObserver<int>
		, public  std::enable_shared_from_this<ScoreHUD>
	{
	public:
		ScoreHUD(std::shared_ptr<ScoreComponent> scoreComponent
			, std::shared_ptr<TextComponent> m_scoreText);

		virtual ~ScoreHUD() = default;
		ScoreHUD(const ScoreHUD& other) = delete;
		ScoreHUD(ScoreHUD&& other) = delete;
		ScoreHUD& operator=(const ScoreHUD& other) = delete;
		ScoreHUD& operator=(ScoreHUD&& other) = delete;

		void Update() override;
		void Render() const override;
		void Notify(const int& score) override;

	private:
		std::shared_ptr<TextComponent> m_scoreText;

		std::shared_ptr<ScoreComponent> m_scoreComponent;

		void Start() override;
	};
}

