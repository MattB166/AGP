#pragma once
#include <Keyboard.h>
#include <Mouse.h>
#include <unordered_map>
#include "IInputManager.h"
using namespace DirectX;
enum class MouseButton
{
	Left,
	Right,
	Middle
};
class KeyboardMouse : public IInputManager
{
public: 
	KeyboardMouse();

	~KeyboardMouse();

	void Update() override;

	void CleanUp() override;

	void BindKeyToFunction(int key, BindingData data) override;

	//void BindAxisToFunction(const std::string& axis, const std::function<void(float)>& action) override;

	void Initialise() override;

	void ClearAllBindings() override;

    void BindKeyToFunction(Keyboard::Keys key, BindingData data);

    void BindMouseToFunction(MouseButton button, BindingData data);

	void ProcessInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;



private:
	std::unique_ptr<Keyboard> m_keyboard = nullptr;

	std::unique_ptr<Mouse> m_mouse = nullptr;

	 std::unordered_map<int, BindingData> m_keyBindings;

	 std::unordered_map<MouseButton, BindingData> m_mouseBindings;

	 DirectX::Keyboard::State m_PrevkeyboardState;

	 DirectX::Mouse::State m_PrevmouseState;

	 KeyState GetKeyState(DirectX::Keyboard::State& currentState, DirectX::Keyboard::Keys key);

	 KeyState GetMouseState(DirectX::Mouse::State& currentState, MouseButton button);


};

