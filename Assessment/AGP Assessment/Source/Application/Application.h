#pragma once
#include <Windows.h>
#include "../Window/Window.h"
class Application
{
public:
	Application(); 
	~Application(); 

	bool Initialize(HINSTANCE hInstance, int nCmdShow); //initialize window, renderer, imgui. 
	void Run();  //run the application if initialized.


private:
	Window* m_window = nullptr;

};

