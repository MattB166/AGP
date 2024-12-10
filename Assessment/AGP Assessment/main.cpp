#include <Windows.h>
#include <d3d11.h>
#include <iostream>

HINSTANCE hInstance = NULL;
HWND hwnd = NULL;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) 
{
	//left off here, need to give program an "application" instance to run. 

	//so "Application" instance - > run. 
	//application holds window, renderer and imgui. 

	//IMGUI for scene editing. can move objects around, change their properties, etc. 
	//when "play" scene, IMGUI disappears, and renderer will render the scene and all behaviours will be updated, as "game loop" will start. when exit clicked, or escape key pressed, game loop will stop, and IMGUI will reappear.

	return 0; 
}

	