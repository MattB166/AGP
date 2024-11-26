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

	static Keyboard& GetKeyboard() { static Keyboard m_KeyBoard; return m_KeyBoard; }
	static Mouse& GetMouse() { static Mouse m_Mouse; return m_Mouse; }

	static Keyboard::State& GetKeyBoardState() { static Keyboard::State m_keyBoardState; return m_keyBoardState; }
	static Keyboard::KeyboardStateTracker& GetKeyBoardTracker() { static Keyboard::KeyboardStateTracker m_keyBoardTracker; return m_keyBoardTracker; }
	static Mouse::State& GetMouseState() { static Mouse::State m_mouseState; return m_mouseState; }

	static std::unordered_map<Keyboard::Keys, BindingData>& GetKeyBindings() { static std::unordered_map<Keyboard::Keys, BindingData> m_keyBindings; return m_keyBindings; }
	static std::unordered_map<MouseButton, BindingData>& GetMouseBindings() { static std::unordered_map<MouseButton, BindingData> m_mouseBindings; return m_mouseBindings; }

	static void ProcessKeyboardInput();
	static void ProcessMouseInput(); 
};

