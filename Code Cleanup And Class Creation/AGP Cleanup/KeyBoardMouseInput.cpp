#include "KeyBoardMouseInput.h"

void KeyBoardMouseInput::Update()
{
	m_kbState = m_keyboard.GetState();
	m_mouseState = m_mouse.GetState();
	
	m_keyboardTracker.Update(m_kbState);
	ProcessKeyboardInput();
	ProcessMouseInput();
}

void KeyBoardMouseInput::CleanUp()
{

}

void KeyBoardMouseInput::Initialise()
{

}

void KeyBoardMouseInput::ClearAllBindings()
{
	m_keyBindings.clear();
	m_mouseBindings.clear();

}

void KeyBoardMouseInput::BindKeyToFunction(Keyboard::Keys key, BindingData data)
{
	m_keyBindings[key] = data;
}

void KeyBoardMouseInput::BindMouseToFunction(MouseButton button, BindingData data)
{
	m_mouseBindings[button] = data;

}


void KeyBoardMouseInput::ProcessKeyboardInput()
{
	for (auto& key : m_keyBindings)
	{
		switch (key.second.PressState)
		{
		case State::Held:
			if (m_kbState.IsKeyDown(key.first))
			{
				key.second.action();
			}
			break;
		case State::Pressed:
			if (m_kbState.IsKeyDown(key.first) && !m_keyboardTracker.IsKeyPressed(key.first))
			{
				key.second.action();
			}
			break;
		case State::Released:
			if (!m_kbState.IsKeyDown(key.first) && m_keyboardTracker.IsKeyPressed(key.first))
			{
				key.second.action();
			}
			break;
		default:
			break;
		}
	}
}



