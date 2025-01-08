#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include "../AGP Assessment/Source/Application/Application.h"

HINSTANCE hInstance = NULL;
HWND hwnd = NULL;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) 
{

	Application app;
	if (FAILED(app.Initialize(hInstance, nCmdShow)))
	{
		MessageBox(NULL, L"Failed to initialize application", L"Error", MB_OK);
		return 0;
	}
	app.Run(); 

	return 0; 
}

	