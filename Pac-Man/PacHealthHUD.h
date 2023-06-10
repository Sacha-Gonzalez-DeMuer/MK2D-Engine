#pragma once
#include "Component.h"
#include "IObserver.h"

namespace dae
{
	class TextComponent;
	class PacHealthComponent;
	class Font;

	class PacHealthHUD final
		: public Component
	{
	public:
		PacHealthHUD(std::shared_ptr<PacHealthComponent> health);

		virtual ~PacHealthHUD() = default;
		PacHealthHUD(const PacHealthHUD& other) = delete;
		PacHealthHUD(PacHealthHUD&& other) = delete;
		PacHealthHUD& operator=(const PacHealthHUD& other) = delete;
		PacHealthHUD& operator=(PacHealthHUD&& other) = delete;

		void Update() override;
		void Render() const override;

		void UpdateHUD() const;

	private:
		std::shared_ptr<PacHealthComponent> m_healthComponent;
		std::shared_ptr<TextComponent> m_livesText;

		void Start() override;
	};
}

