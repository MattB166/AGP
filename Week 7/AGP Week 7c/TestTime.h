#pragma once
#include <chrono>
class TestTime
{
public:

	static void Tick();
	static void SetTimeScale(float scale);
	static float getDeltaTime();
	static float getElapsedTime();
	static void CalculateFrameStats();
private:
	static float timeScale;
	static float deltaTime;
	static float elapsedTime;
	static int frameCount;
	static float FPS;
	static std::chrono::system_clock::time_point currentTime;
	static std::chrono::system_clock::time_point previousTime;
	TestTime();
	~TestTime();

};

