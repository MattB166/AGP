#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include "Window.h"
#include "Renderer.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600



////global variables


//IDXGISwapChain* g_swapChain = NULL;             // the pointer to the swap chain interface - this is a chain of screen buffers that we draw to and "flip" to display on the monitor. 
//ID3D11Device* g_dev = NULL;                     // the pointer to our Direct3D device interface - the virtual display adapter aka graphics card driver. this object is used to create and allocate resources on the adapter. 
//ID3D11DeviceContext* g_devcon = NULL;           // the pointer to our Direct3D device context - used to control the settings and states of the device, and issue rendering commands to the device.


////FORWARD DECLARATIONS FOR FUNCTION PROTOTYPES

//HRESULT InitD3D(HWND hWnd);  ///Initialize Direct3D 
//void CleanD3D();  ///Clean up Direct3D resources
//void OpenConsole();  ///Open a console window for debugging purposes

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) ///returns an int value. OS doesnt use the return value but can use this
///value to pass a status code onto another program that is waiting for the completion of this program 
{
	Window w;
	
	w.OpenConsole();

	if (FAILED(w.InitWindow(hInstance, nCmdShow)))
	{
		MessageBox(NULL, L"Window Initialization Failed!", L"Error!", MB_ICONERROR | MB_OK);
	}

	Renderer r;
	

	if (FAILED(r.InitRenderer(w.GetWindow(),SCREEN_HEIGHT,SCREEN_WIDTH)))
	{
		MessageBox(NULL, L"Renderer Initialization Failed!", L"Error!", MB_ICONERROR | MB_OK);
	}

	w.Run();

	

	r.CleanRenderer();

	return 0;
}
