#include "TestTime.h"
#include <iostream>
std::chrono::system_clock::time_point TestTime::currentTime = std::chrono::system_clock::now();
std::chrono::system_clock::time_point TestTime::previousTime = std::chrono::system_clock::now();
static std::chrono::system_clock::time_point previousTime;
float TestTime::deltaTime = 0;
float TestTime::timeScale = 1;
float TestTime::elapsedTime = 0;
int TestTime::frameCount = 0;
float TestTime::FPS = 0;

void TestTime::Tick()
{
	previousTime = currentTime;
	currentTime = std::chrono::system_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(currentTime - previousTime).count() / 1000.0f;
	elapsedTime += deltaTime;
	//std::cout << "Delta Time: " << deltaTime << std::endl;
}

void TestTime::SetTimeScale(float scale)
{
	timeScale = scale;
}

float TestTime::getDeltaTime()
{
	return (timeScale * deltaTime);
}

float TestTime::getElapsedTime()
{
	return elapsedTime;
}

void TestTime::CalculateFrameStats()
{
	frameCount++;
	elapsedTime += deltaTime;
	if (elapsedTime >= 1.0f)
	{
		FPS = frameCount;
		//std::cout << "FPS: " << FPS << std::endl;
		//std::cout << "Delta Time: " << deltaTime << std::endl;
		frameCount = 0;
		elapsedTime = 0.0f;
	}
}

TestTime::TestTime()
{
	previousTime = std::chrono::system_clock::now();
	currentTime = std::chrono::system_clock::now();
}

TestTime::~TestTime()
{

}
