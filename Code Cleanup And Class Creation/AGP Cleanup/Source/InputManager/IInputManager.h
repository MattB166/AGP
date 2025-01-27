#pragma once
#include <functional>
enum class State
{
	Held,
	Pressed,
	Released,
	None
};
struct BindingData
{
	std::function<void()> action;
	State PressState;

	BindingData() : action(nullptr), PressState(State::None) {}
	BindingData(std::function<void()> action, State PressState) : action(action), PressState(PressState) {}
};

class IInputManager
{

public:
	virtual ~IInputManager() = default;

	virtual void Update() = 0;

	virtual void Initialise() = 0; 

	virtual void BindKeyToFunction(int key, BindingData data) = 0;

	//virtual void BindAxisToFunction(const std::string& axis, const std::function<void(float)>& action) = 0;

	virtual void CleanUp() = 0;

	virtual void ClearAllBindings() = 0;



};

