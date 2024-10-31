#pragma once
#include <Windows.h>
#include <chrono>
class TimeClass
{
public:
	static void Tick();
	static void Initialise();

	static inline float GetDeltaTime() { return deltaTime; }

	static void CalculateFrameStats(float deltaTime);

private:

	static __int64 countsPerSecond;
	static float secsPerCount; ///1.0f / countsPerSecond

	static __int64 previousTime;
	static __int64 currentTime;
	static float deltaTime;
	static float elapsedTime;
	static int frameCount;
	static float fps;

	TimeClass() = delete;
	~TimeClass() = delete;
	TimeClass(const TimeClass&) = delete;
	TimeClass& operator=(const TimeClass&) = delete;
};

