#pragma once
#include "Component.h"
#include "IObserver.h"

namespace dae
{
	class TextComponent;
	class PacScoreComponent;
	class Font;
	class PacScoreHUD final
		: public Component
		, public std::enable_shared_from_this<PacScoreHUD>
	{
	public:
		PacScoreHUD(std::shared_ptr<PacScoreComponent> scoreComponent);

		virtual ~PacScoreHUD() = default;
		PacScoreHUD(const PacScoreHUD& other) = delete;
		PacScoreHUD(PacScoreHUD&& other) = delete;
		PacScoreHUD& operator=(const PacScoreHUD& other) = delete;
		PacScoreHUD& operator=(PacScoreHUD&& other) = delete;

		void Start() override;
		void Update() override;
		void Render() const override;

		void UpdateText() const;

	private:
		std::shared_ptr<TextComponent> m_scoreText;
		std::shared_ptr<PacScoreComponent> m_scoreComponent;
	};
}

