#include "Button.h"
#include <SDL.h>
#include "Commands.h"

dae::Button::Button()
	: RenderComponent(), m_btnMinX{ 0 }, m_btnMaxX{ 0 }, m_btnMinY{ 0 }, m_btnMaxY{ 0 }
{
}

void dae::Button::Update()
{
	int mouse_x, mouse_y;
	const Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		if (mouse_x > m_btnMinX && mouse_x < m_btnMaxX && mouse_y > m_btnMinY && mouse_y < m_btnMaxY)
		{
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
