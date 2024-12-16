#pragma once
#include <chrono>
class TimeClass
{
public:
	static void Tick();
	static void SetTimeScale(float timeScale);
	static float GetDeltaTime();
	static float GetElapsedTime();
	static void CalculateFrameStats();
private:
	static float m_deltaTime;
	static float m_elapsedTime;
	static float m_timeScale;
	static int m_frameCount;
	static float m_fps;

	static std::chrono::system_clock::time_point m_currentTime;
	static std::chrono::system_clock::time_point m_previousTime;
	TimeClass();
	~TimeClass();
};

