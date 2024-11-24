#pragma once
#include <functional>

enum class State
{
	Held,
	Pressed,
	Released,
};
class IInputManager
{
public:

	virtual ~IInputManager() = default; 

	virtual void Init() = 0;

	virtual void Update() = 0;





};

