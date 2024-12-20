#pragma once
#include <functional>
#include "Windows.h"



class IInputManager
{
public:
	virtual ~IInputManager() = default;

	virtual void Update() = 0;

	virtual void Initialise() = 0;

	//virtual void BindKeyToFunction(int key, BindingData data) = 0;

	//virtual void BindAxisToFunction(const std::string& axis, const std::function<void(float)>& action) = 0;

	virtual void CleanUp() = 0;

	virtual void ClearAllBindings() = 0;

	virtual void ProcessInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

	virtual const void* GetCurrentState() const = 0;
};

