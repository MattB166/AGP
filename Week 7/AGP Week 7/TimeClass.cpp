#include "TimeClass.h"
#include <iostream>
float TimeClass::deltaTime = 0.0f;
float TimeClass::secsPerCount = 0;
__int64 TimeClass::previousTime = 0;
__int64 TimeClass::currentTime = 0;
__int64 TimeClass::countsPerSecond = 0;

void TimeClass::Tick()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
    deltaTime = (currentTime - previousTime) * secsPerCount;
    previousTime = currentTime;

  
    std::cout << "Delta Time: " << deltaTime << std::endl;
    std::cout << "Previous Time: " << previousTime << std::endl;
    std::cout << "Current Time: " << currentTime << std::endl;
	
}

void TimeClass::Initialise()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
    secsPerCount = 1.0f / countsPerSecond;

    QueryPerformanceCounter((LARGE_INTEGER*)&previousTime);
    currentTime = previousTime; // Initialize currentTime to the same value as previousTime

  /*  std::cout << "Counts Per Second: " << countsPerSecond << std::endl;
    std::cout << "Secs Per Count: " << secsPerCount << std::endl;
    std::cout << "Initial Previous Time: " << previousTime << std::endl;
    std::cout << "Initial Current Time: " << currentTime << std::endl;*/
}
