#include "FPSCounter.h"
#include "Time.h"
#include "TextObject.h"
#include "GameObject.h"

void dae::FPSCounter::Start()
{
	m_textObject = m_gameObject.lock()->GetComponent<TextComponent>();
}

void dae::FPSCounter::Update()
{
    // Calculate the FPS for the current frame
    const float fps = 1.0f / Time::GetInstance().DeltaTime();

    // Update the rolling average of the FPS
    const float alpha = 0.9f; // The weight to give the new value
    m_framesPerSecond = alpha * m_framesPerSecond + (1.0f - alpha) * fps;

    // Update the text object with the new FPS value
    m_textObject->SetText(std::to_string(static_cast<int>(m_framesPerSecond)));
}

void dae::FPSCounter::Render() const
{
	m_textObject->Render();
}
