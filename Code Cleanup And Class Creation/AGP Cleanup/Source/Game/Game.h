#pragma once
#include "../GameObject/GameObject.h"
#include "../Window/Window.h"
#include "../InputManager/IInputManager.h"
#include"../Renderer/Renderer.h"
class Game
{

public:
	Game();
	bool Init(); //function which initializes the game and renderer 
	void Run(); //function which runs the game loop  
	void SetupInputs(std::unique_ptr<IInputManager> InputType); //function which sets up the inputs for the game 
	void Update(); //function which updates the game
	void LoadScene(); //function which loads the scene 

private: 
	static Renderer* r; 
	Window w; 
	std::unique_ptr<IInputManager> inputManager;

	//vector of scenes ? scene has skybox , game objects, lights, camera etc. 
	//std::vector<GameObject*> GameObjects; //might not be needed. will go into scene class 
};

