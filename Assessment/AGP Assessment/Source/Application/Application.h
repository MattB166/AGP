#pragma once
#include <Windows.h>
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
class Application
{
public:
	Application(); 
	~Application(); 

	bool Initialize(HINSTANCE hInstance, int nCmdShow); //initialize window, renderer, imgui. 
	void Run();  //run the application if initialized.


private:
	Window* m_window = nullptr;
	Renderer* m_renderer = nullptr; 
	//list/vector of scenes. 

	//need a way to determine whether we are in "edit" mode or "play" mode. 
};

