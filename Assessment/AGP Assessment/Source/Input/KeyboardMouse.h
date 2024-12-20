#pragma once
#include <Keyboard.h>
#include <Mouse.h>
#include <unordered_map>
#include <functional>
using namespace DirectX;
enum class KeyState
{
	Held,
	Pressed,
	Released,
	None
};
struct BindingData
{
	std::function<void()> action;
	KeyState PressState;

	BindingData() : action(nullptr), PressState(KeyState::None) {}
	BindingData(std::function<void()> action, KeyState PressState) : action(action), PressState(PressState) {}
};
enum class MouseButton
{
	Left,
	Right,
	Middle
};
class KeyboardMouse
{
public: 
	KeyboardMouse();

	~KeyboardMouse();

	void Update();

	void CleanUp();

	void Initialise();

	void ClearAllBindings();

    void BindKeyToFunction(Keyboard::Keys key, BindingData data);

    void BindMouseToFunction(MouseButton button, BindingData data);

	void ProcessKeyboardInput(const DirectX::Keyboard::State& currentState);

	void ProcessMouseInput(const DirectX::Mouse::State& currentState);


	DirectX::Keyboard::State GetKeyboardState() const;

	DirectX::Keyboard::State GetPrevKeyboardState() const;





private:
	

	 //UP TO DATE ONES HERE
	 std::unordered_map<DirectX::Keyboard::Keys, BindingData> keyBindings;
	 std::unordered_map < DirectX::Mouse::ButtonStateTracker::ButtonState, BindingData> mouseBindings;

	 DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
};

