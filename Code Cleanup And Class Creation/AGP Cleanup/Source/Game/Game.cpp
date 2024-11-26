#include "Game.h"
#include <iostream>
Renderer* Game::r = nullptr;

void Game::Run()
{
}

void Game::SetupInputs(std::unique_ptr<IInputManager> InputType)
{
	inputManager = std::move(InputType);
	inputManager->Initialise(); 
	inputManager->BindKeyToFunction(Keyboard::Keys::W, BindingData([]() {std::cout << "W Pressed" << std::endl; }, State::Pressed));
}

void Game::Update()
{
	inputManager->Update(); 
	//call game loop and render frame with updated game objects and positions 
}
