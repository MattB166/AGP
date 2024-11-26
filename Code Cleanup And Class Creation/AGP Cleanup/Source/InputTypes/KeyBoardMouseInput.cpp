#include "KeyBoardMouseInput.h"

void KeyBoardMouseInput::Update()
{
	auto kbState = GetKeyboard().GetState();
	GetKeyBoardTracker().Update(kbState);
	auto mouseState = GetMouse().GetState();

	ProcessKeyboardInput();
	ProcessMouseInput(); 
}

void KeyBoardMouseInput::CleanUp()
{
	delete& GetKeyboard();
	delete& GetMouse();
	delete& GetKeyBoardState();
	delete& GetKeyBoardTracker();
	delete& GetMouseState();
	delete& GetKeyBindings();
	delete& GetMouseBindings();

}

void KeyBoardMouseInput::BindKeyToFunction(int key, BindingData data)
{
	BindKeyToFunction(static_cast<Keyboard::Keys>(key), data); //will this work?? 
}

//void KeyBoardMouseInput::BindAxisToFunction(const std::string& axis, const std::function<void(float)>& action)
//{
//	GetAxisFunction(action); 
//}

void KeyBoardMouseInput::Initialise()
{
	
}

void KeyBoardMouseInput::ClearAllBindings()
{
	GetKeyBindings().clear();
	GetMouseBindings().clear();
}

void KeyBoardMouseInput::BindKeyToFunction(Keyboard::Keys key, BindingData data)
{
	GetKeyBindings()[key] = data; //might not work?? might need to insert pair 
}

void KeyBoardMouseInput::BindMouseToFunction(MouseButton button, BindingData data)
{
	GetMouseBindings()[button] = data;
}

void KeyBoardMouseInput::ProcessKeyboardInput()
{
	for (auto& key : GetKeyBindings())
	{
		switch (key.second.PressState)
		{
		case State::Held:
			if (GetKeyBoardState().IsKeyDown(key.first))
			{
				key.second.action();
			}
			break;
		case State::Pressed:
			if (GetKeyBoardState().IsKeyDown(key.first) && !GetKeyBoardTracker().IsKeyPressed(key.first))
			{
				key.second.action();
			}
			break;
		case State::Released:
			if (!GetKeyBoardState().IsKeyDown(key.first) && GetKeyBoardTracker().IsKeyPressed(key.first))
			{
				key.second.action();
			}
			break;
			default
				:
					break;

		}
	}
}

void KeyBoardMouseInput::ProcessMouseInput()
{
}
