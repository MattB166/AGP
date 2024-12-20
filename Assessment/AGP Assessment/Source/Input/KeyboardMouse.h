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
	int ID;

	BindingData() : action(nullptr), PressState(KeyState::None), ID(0) {}
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

	void ClearTempBindings();

	void ClearKeyBinding(Keyboard::Keys key);

	void ClearMouseBinding(MouseButton button);

	void ClearMouseMovement();

    void BindKeyToFunction(Keyboard::Keys key, BindingData data);

    void BindMouseToFunction(MouseButton button, BindingData data);

	void BindMouseMovement(std::function<void(int, int)> action);

	void ProcessKeyboardInput(const DirectX::Keyboard::State& currentState);

	void ProcessMouseInput(const DirectX::Mouse::State& currentState);


	DirectX::Keyboard::State GetKeyboardState() const;

	DirectX::Keyboard::State GetPrevKeyboardState() const;

	DirectX::Keyboard::Keys GetKeyFromBinding(std::function<void()> func);





private:
	

	 //UP TO DATE ONES HERE
	 std::unordered_map<DirectX::Keyboard::Keys, BindingData> keyBindings;
	 std::unordered_map <MouseButton, BindingData> mouseBindings;
	 std::function<void(int, int)> mouseMovementAction;

	 DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	 DirectX::Mouse::ButtonStateTracker m_mouseTracker;



};

