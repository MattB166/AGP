#pragma once
#include "../GameObject/GameObject.h"
#include"../Renderer/Renderer.h"
class Game
{
	//ref to renderer and render all the game objects 
public:
	std::vector<GameObject*> Objects; 


private: 
	Renderer* r; 
};

