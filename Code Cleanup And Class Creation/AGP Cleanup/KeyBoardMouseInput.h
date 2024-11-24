#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <unordered_map>
#include <Mouse.h>
#include <Keyboard.h>
#include "IInputManager.h"
using namespace DirectX;
enum class MouseButton
{
	Left,
	Right,
	Middle
};
class KeyBoardMouseInput :   //mouse currently not implemented as cant get access to the buttons 
    public IInputManager
{
public:
	KeyBoardMouseInput() = default;
	~KeyBoardMouseInput() = default;

	void Update() override;
	void CleanUp() override;
	void Initialise() override;
	void ClearAllBindings() override;


	static void BindKeyToFunction(Keyboard::Keys key, BindingData data);

	static void BindMouseToFunction(MouseButton button, BindingData data);



private:

	static Keyboard m_keyboard;
	static Mouse m_mouse;

	static Keyboard::State m_kbState; //current state of the keyboard
	static Keyboard::KeyboardStateTracker m_keyboardTracker;
	static Mouse::State m_mouseState; //current state of the mouse

	static std::unordered_map<Keyboard::Keys, BindingData> m_keyBindings;
	static std::unordered_map<MouseButton, BindingData> m_mouseBindings;

	static void ProcessKeyboardInput();
	static void ProcessMouseInput(); 
};

