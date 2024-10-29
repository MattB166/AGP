#pragma once
#include <Windows.h>
#include <chrono>
class TimeClass
{
public:
	static void Tick();
	static void Initialise();

	static inline float GetDeltaTime() { return deltaTime; }

private:

	static __int64 countsPerSecond;
	static float secsPerCount; ///1.0f / countsPerSecond

	static __int64 previousTime;
	static __int64 currentTime;
	static float deltaTime;

	TimeClass() = delete;
	~TimeClass() = delete;
	TimeClass(const TimeClass&) = delete;
	TimeClass& operator=(const TimeClass&) = delete;
};

