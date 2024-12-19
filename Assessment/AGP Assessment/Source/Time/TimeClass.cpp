#include "TimeClass.h"
#include <iostream>
std::chrono::system_clock::time_point TimeClass::m_currentTime;
std::chrono::system_clock::time_point TimeClass::m_previousTime;
float TimeClass::m_deltaTime;
float TimeClass::m_elapsedTime;
float TimeClass::m_timeScale = 1;
int TimeClass::m_frameCount;
float TimeClass::m_fps;
TimeClass::TimeClass()
{
	m_previousTime = std::chrono::system_clock::now();
	m_currentTime = std::chrono::system_clock::now();
}

TimeClass::~TimeClass()
{
}

void TimeClass::Tick()
{
	m_previousTime = m_currentTime;
	m_currentTime = std::chrono::system_clock::now();
	m_deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(m_currentTime - m_previousTime).count() / 1000.0f;
	m_elapsedTime += m_deltaTime;
}

void TimeClass::SetTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}

float TimeClass::GetDeltaTime()
{
	return (m_timeScale * m_deltaTime);
}

float TimeClass::GetElapsedTime()
{
	return m_elapsedTime;
}

void TimeClass::CalculateFrameStats()
{
	m_frameCount++;
	m_elapsedTime += m_deltaTime;
	if (m_elapsedTime >= 1.0f)
	{
		m_fps = static_cast<float>(m_frameCount);
		std::cout << "FPS: " << m_fps << std::endl;
		m_frameCount = 0;
		m_elapsedTime = 0.0f;
	}
}
