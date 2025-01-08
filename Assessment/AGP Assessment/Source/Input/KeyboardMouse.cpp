#include "KeyboardMouse.h"
#include <iostream>

KeyboardMouse::KeyboardMouse()
{
}

KeyboardMouse::~KeyboardMouse()
{
	CleanUp();
}



void KeyboardMouse::Update()
{
	
}

void KeyboardMouse::CleanUp()
{
	keyBindings.clear();
	mouseBindings.clear();
	mouseMovementAction = nullptr;

}

void KeyboardMouse::Initialise()
{
	/*m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();*/
}

void KeyboardMouse::ClearAllBindings()
{
	if (keyBindings.empty() && mouseBindings.empty())
	{
		return;
	}
	keyBindings.clear();
	mouseBindings.clear();
	//std::cout << "All bindings cleared" << std::endl;
	//std::cout << "KeyBindings size: " << keyBindings.size() << std::endl;
	//std::cout << "MouseBindings size: " << mouseBindings.size() << std::endl;
}

void KeyboardMouse::ClearKeyBinding(Keyboard::Keys key)
{
	auto it = keyBindings.find(key);
	if (it != keyBindings.end())
	{
		keyBindings.erase(it);
		//std::cout << "Key Binding removed" << std::endl;
	}
	else
	{
		//std::cerr << "Key Binding not found" << std::endl;
	}
}

void KeyboardMouse::ClearMouseBinding(MouseButton button)
{
	auto it = mouseBindings.find(button);
	if (it != mouseBindings.end())
	{
		mouseBindings.erase(it);
		//std::cout << "Mouse Binding removed" << std::endl;
	}
	else
	{
		//std::cerr << "Mouse Binding not found" << std::endl;
	}
}

void KeyboardMouse::ClearMouseMovement()
{
	mouseMovementAction = nullptr;
	//std::cout << "Mouse Movement Binding removed" << std::endl;

}

void KeyboardMouse::BindKeyToFunction(Keyboard::Keys key, BindingData data)
{
	static int nextID = 1;
	data.ID = nextID++;
	keyBindings.insert(std::make_pair(key, data));
	//std::cout << "Key Binding added" << std::endl;
}

void KeyboardMouse::BindMouseToFunction(MouseButton button, BindingData data)
{
	static int nextID = 1;
	data.ID = nextID++;
	mouseBindings.insert(std::make_pair(button, data));
	//std::cout << "Mouse Binding added" << std::endl;
}

void KeyboardMouse::BindMouseMovement(std::function<void(int, int)> action)
{
	mouseMovementAction = action;
	//std::cout << "Mouse Movement Binding added" << std::endl;
}


void KeyboardMouse::ProcessKeyboardInput(const DirectX::Keyboard::State& currentState)
{
	m_keyboardTracker.Update(currentState);
	if (keyBindings.empty())
	{
		return;
	}

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
		default:
			std::cerr << "Invalid PressState detected" << std::endl;
			continue;
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
			default:
				std::cerr << "Invalid MouseButton detected in ProcessMouseInput" << std::endl;
				continue;
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
			default:
				std::cerr << "Invalid MouseButton detected in ProcessMouseInput" << std::endl;
				continue;
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
			default:
				std::cerr << "Invalid MouseButton detected in ProcessMouseInput" << std::endl;
				continue;
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
	if (mouseMovementAction)
	{
		mouseMovementAction(currentState.x , currentState.y);
		//std::cout << currentState.x << " " << currentState.y << std::endl;
	}
	else
	{
		//std::cout << "No mouse movement action bound" << std::endl;
	}
}

DirectX::Keyboard::Keys KeyboardMouse::GetKeyFromBinding(std::function<void()> func)
{
	//searches through mappings for the key that matches the function 
	int funcID = -1;
	for (const auto& binding : keyBindings)
	{
		if (binding.second.action.target_type() == func.target_type())
		{	
			std::cout << "Function found" << std::endl;
			funcID = binding.second.ID;
			break;
		}
	}
	if (funcID == -1)
	{
		return DirectX::Keyboard::Keys::None;
	}

	for (const auto& binding : keyBindings)
	{
		if (binding.second.ID == funcID)
		{
			std::cout << "Key found" << std::endl;
			return binding.first;
		}
	}

	return DirectX::Keyboard::Keys::None;
}



