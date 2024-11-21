#pragma once
#include "../GameObject/GameObject.h"
#include"../Renderer/Renderer.h"
class Game
{

public:
	Game(Renderer* r);
	void Run(); 
	void AddGameObject(GameObject* go);
	void FreezeGame(); // set time scale to 0; 
	void UnFreezeGame(); // set time scale to 1;
	void EndGame(); // set bIsRunning to false;
	void Loop(); // all game objects update and logic functions are called here. 

private: 
	Renderer* r; 
	bool bIsRunning = true;
	std::vector<GameObject*> GameObjects; 
};

