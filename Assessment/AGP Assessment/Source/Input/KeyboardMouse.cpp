#include "KeyboardMouse.h"
#include <iostream>

KeyboardMouse::KeyboardMouse() : m_keyboard(std::make_unique<DirectX::Keyboard>()), m_mouse(std::make_unique<DirectX::Mouse>()), m_PrevkeyboardState(m_keyboard->GetState()), m_PrevmouseState(m_mouse->GetState())
{
}

KeyboardMouse::~KeyboardMouse()
{
	
}



void KeyboardMouse::Update()
{
	m_PrevkeyboardState = m_CurrentkeyboardState;
	//m_PrevmouseState = m_CurrentmouseState;

	// Get the current state
	m_CurrentkeyboardState = m_keyboard->GetState();
	//m_CurrentmouseState = m_mouse->GetState();

	// Update the trackers
	m_kBTracker.Update(m_CurrentkeyboardState);
	//m_mouseTracker.Update(m_CurrentmouseState);

	// Process keyboard input
	for (const auto& binding : m_keyBindings)
	{
		if (m_kBTracker.IsKeyPressed(static_cast<DirectX::Keyboard::Keys>(binding.first)) && binding.second.PressState == KeyState::Pressed)
		{
			std::cout << "Performing action" << std::endl;
			binding.second.action();
		}
		else if (m_kBTracker.IsKeyReleased(static_cast<DirectX::Keyboard::Keys>(binding.first)) && binding.second.PressState == KeyState::Released)
		{
			binding.second.action();
		}
	}

}

void KeyboardMouse::CleanUp()
{
	

}

void KeyboardMouse::BindKeyToFunction(int key, BindingData data)
{
	BindKeyToFunction(static_cast<Keyboard::Keys>(key), data);
}

void KeyboardMouse::Initialise()
{
	/*m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();*/
}

void KeyboardMouse::ClearAllBindings()
{
	m_keyBindings.clear();
	m_mouseBindings.clear();
}

void KeyboardMouse::BindKeyToFunction(Keyboard::Keys key, BindingData data)
{
	m_keyBindings.insert(std::make_pair(static_cast<int>(key), data));
}

void KeyboardMouse::BindMouseToFunction(MouseButton button, BindingData data)
{
	m_mouseBindings.insert(std::make_pair(button, data));
}

void KeyboardMouse::ProcessInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		//std::cout << "Key Down" << std::endl; 
		if (m_keyBindings.find(wParam) != m_keyBindings.end())
		{
			auto& binding = m_keyBindings[wParam];
			if (binding.PressState == KeyState::Pressed)
			{
				binding.action();
			}
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (m_keyBindings.find(wParam) != m_keyBindings.end())
		{
			auto& binding = m_keyBindings[wParam];
			if (binding.PressState == KeyState::Released)
			{
				binding.action();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		//std::cout << "Left Button Down" << std::endl;
		if (m_mouseBindings.find(MouseButton::Left) != m_mouseBindings.end())
		{
			auto& binding = m_mouseBindings[MouseButton::Left];
			if (binding.PressState == KeyState::Pressed)
			{
				binding.action();
			}
		}
		break;
	case WM_LBUTTONUP:
		//std::cout << "Left Button Up" << std::endl;
		if (m_mouseBindings.find(MouseButton::Left) != m_mouseBindings.end())
		{
			auto& binding = m_mouseBindings[MouseButton::Left];
			if (binding.PressState == KeyState::Released)
			{
				binding.action();
			}
		}
		break;
	case WM_RBUTTONDOWN:
		//std::cout << "Right Button Down" << std::endl;
		if (m_mouseBindings.find(MouseButton::Right) != m_mouseBindings.end())
		{
			auto& binding = m_mouseBindings[MouseButton::Right];
			if (binding.PressState == KeyState::Pressed)
			{
				binding.action();
			}
		}
		break;
	case WM_RBUTTONUP:
		//std::cout << "Right Button Up" << std::endl;
		if (m_mouseBindings.find(MouseButton::Right) != m_mouseBindings.end())
		{
			auto& binding = m_mouseBindings[MouseButton::Right];
			if (binding.PressState == KeyState::Released)
			{
				binding.action();
			}
		}
		break;
	}
}

DirectX::Keyboard::State KeyboardMouse::GetKeyboardState() const
{
	return m_CurrentkeyboardState;
}

DirectX::Keyboard::State KeyboardMouse::GetPrevKeyboardState() const
{
	return m_PrevkeyboardState;
}

const void* KeyboardMouse::GetCurrentState() const
{
	return &m_CurrentkeyboardState;
}


KeyState KeyboardMouse::GetKeyState(DirectX::Keyboard::State& currentState, DirectX::Keyboard::Keys key)
{
	bool isPressed = currentState.IsKeyDown(key);	
	bool wasPressed = m_PrevkeyboardState.IsKeyDown(key);

	if (isPressed && !wasPressed)
	{
		return KeyState::Pressed;
	}
	else if (isPressed && wasPressed)
	{
		return KeyState::Held;
	}
	else if (!isPressed && wasPressed)
	{
		return KeyState::Released;
	}
	else
	{
		return KeyState::None;
	}
}

KeyState KeyboardMouse::GetMouseState(DirectX::Mouse::State& currentState, MouseButton button)
{
	bool isPressed = false;
	bool wasPressed = false;

	switch (button)
	{
	case MouseButton::Left:
		isPressed = currentState.leftButton;
		wasPressed = m_PrevmouseState.leftButton;
		break;
	case MouseButton::Right:
		isPressed = currentState.rightButton;
		wasPressed = m_PrevmouseState.rightButton;
		break;
	case MouseButton::Middle:
		isPressed = currentState.middleButton;
		wasPressed = m_PrevmouseState.middleButton;
		break;
	}

	if (isPressed && !wasPressed)
	{
		return KeyState::Pressed;
	}
	else if (isPressed && wasPressed)
	{
		return KeyState::Held;
	}
	else if (!isPressed && wasPressed)
	{
		return KeyState::Released;
	}
	else
	{
		return KeyState::None;
	}
}
