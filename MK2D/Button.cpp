#include "Button.h"
#include <SDL.h>
#include "Commands.h"
#include "GameTime.h"

dae::Button::Button()
	: RenderComponent(), m_btnMinX{ 0 }, m_btnMaxX{ 0 }, m_btnMinY{ 0 }, m_btnMaxY{ 0 }, m_reclickDelay{ 0 }
{
}

void dae::Button::Update()
{
	if (m_reclickDelay > 0)
	{
		m_reclickDelay -= GameTime::Get().DeltaTime();
		return;
	}

	int mouse_x, mouse_y;
	const Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);


	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT) && m_reclickDelay <= 0)
	{
		if (mouse_x > m_btnMinX && mouse_x < m_btnMaxX && mouse_y > m_btnMinY && mouse_y < m_btnMaxY)
		{
			m_reclickDelay = 0.1f;
			OnClick.Invoke();
		}
	}
}

void dae::Button::Start()
{
	const auto& pos = m_gameObject.lock()->GetWorldPosition();

	m_btnMinX = int(pos.x);
	m_btnMaxX = int(pos.x + m_size.x);
	m_btnMinY = int(pos.y);
	m_btnMaxY = int(pos.y + m_size.y);
}
