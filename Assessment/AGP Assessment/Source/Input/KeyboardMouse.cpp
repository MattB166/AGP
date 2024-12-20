#include "KeyboardMouse.h"
#include <iostream>

KeyboardMouse::KeyboardMouse()
{
}

KeyboardMouse::~KeyboardMouse()
{
	
}



void KeyboardMouse::Update()
{
	
}

void KeyboardMouse::CleanUp()
{
	

}

void KeyboardMouse::Initialise()
{
	/*m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();*/
}

void KeyboardMouse::ClearAllBindings()
{
	
}

void KeyboardMouse::BindKeyToFunction(Keyboard::Keys key, BindingData data)
{
	keyBindings.insert(std::make_pair(key, data));
}

void KeyboardMouse::BindMouseToFunction(MouseButton button, BindingData data)
{
	mouseBindings.insert(std::make_pair(button, data));
}


void KeyboardMouse::ProcessKeyboardInput(const DirectX::Keyboard::State& currentState)
{
	m_keyboardTracker.Update(currentState);

	for (const auto& binding : keyBindings)
	{
		bool Condition = false;
		switch (binding.second.PressState)
		{
		case KeyState::Held:
			Condition = currentState.IsKeyDown(binding.first);
			break;
		case KeyState::Pressed:
			Condition = m_keyboardTracker.IsKeyPressed(binding.first);
			break;
		case KeyState::Released:
			Condition = m_keyboardTracker.IsKeyReleased(binding.first);
			break;
		case KeyState::None:
			break;
		}
		if (Condition && binding.second.action)
		{
			binding.second.action();
		}

	}
}

void KeyboardMouse::ProcessMouseInput(const DirectX::Mouse::State& currentState)
{
	m_mouseTracker.Update(currentState);
	for (const auto& binding : mouseBindings)
	{
		bool condition = false;
		switch (binding.second.PressState)
		{
		case KeyState::Held:
			switch (binding.first)
			{
			case MouseButton::Left:
				condition = currentState.leftButton;
				break;
			case MouseButton::Right:
				condition = currentState.rightButton;
				break;
			case MouseButton::Middle:
				condition = currentState.middleButton;
				break;
			}
			break;
		case KeyState::Pressed:
			switch (binding.first)
			{
			case MouseButton::Left:
				condition = m_mouseTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
				break;
			case MouseButton::Right:
				condition = m_mouseTracker.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
				break;
			case MouseButton::Middle:
				condition = m_mouseTracker.middleButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
				break;
			}
			break;
		case KeyState::Released:
			switch (binding.first)
			{
			case MouseButton::Left:
				condition = m_mouseTracker.leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED;
				break;
			case MouseButton::Right:
				condition = m_mouseTracker.rightButton == DirectX::Mouse::ButtonStateTracker::RELEASED;
				break;
			case MouseButton::Middle:
				condition = m_mouseTracker.middleButton == DirectX::Mouse::ButtonStateTracker::RELEASED;
				break;
			}
			break;
		case KeyState::None:
			break;
		}
		if (condition && binding.second.action)
		{
			binding.second.action();
		}
	}
}



