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
///hInstance: Handle to the current instance of the application. OS uses this to identify the executable when it is loaded in memory
///hPrevInstance: Handle to the previous instance of the application - has no meaning in Win32 programming and is always zero 
/// lpCmdLine: Command line arguments as a Unicode string
/// nCmdShow: How the window is to be shown. i.e. maximized, minimized, etc.

//HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow)
//{
//	g_hInst = instanceHandle;  ///store the instance handle in our global variable
//	///First we need to register a window class
//	//This is how windows stores properties of a window that we want to create
//	WNDCLASSEX wc = { };  ///initialize all members of the struct to 0. Can also be achieved with "ZeroMemory(&wc, sizeof(WNDCLASSEX))" macro "
//	///Fill in the struct with the necessary information
//	wc.cbSize = sizeof(WNDCLASSEX);  ///size of the struct
//	wc.style = CS_HREDRAW | CS_VREDRAW;  ///style of the window class
//	wc.lpfnWndProc = WndProc;  ///Our window procedure function, make sure it handles window creation or calls DefWindowProc(..)
//	wc.hInstance = instanceHandle;  ///Give our app's handle 
//	wc.lpszClassName = L"WindowClass1";  ///Windows will store our window class with this name
//	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);  ///Background color of the win32 app. (not needed for D3D apps)
//	//register class with above struct. if it fails, if block will execute
//
//	///ADJUSTING THE WINDOW DIMENSIONS SO THAT THE TOP WINDOWS BAR IS NOT TAKING PIXELS AWAY FROM OUR APP
//	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//	GetClientRect(g_hWnd, &wr);  ///Get the client area of the window that was set previously 
//	UINT width = wr.right - wr.left;
//	UINT height = wr.bottom - wr.top;
//
//	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);  ///Adjust the window size to fit the desired client area size
//
//	if (!RegisterClassEx(&wc))
//	{
//		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
//		return E_FAIL;
//	}
//
//
//	///Create the window and use the result as the handle to the window
//	g_hWnd = CreateWindowEx(NULL,
//		L"WindowClass1", // name of the window class
//		windowName, // title of the window
//		WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU, // window style with no resizing and maximizing 
//		//WS_OVERLAPPEDWINDOW, // window style with resizing and maximizing
//		100, // x-position of the window
//		100, // y-position of the window
//		wr.right - wr.left, // width of the window
//		wr.bottom - wr.top, // height of the window
//		NULL, // we have no parent window, NULL
//		NULL, // we aren't using menus, NULL
//		instanceHandle, // application handle
//		NULL); // used with multiple windows, NULL
//
//	if (g_hWnd == NULL) return E_FAIL;
//
//	ShowWindow(g_hWnd, nCmdShow);  ///Show the window
//	return S_OK;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//		//This message is sent when the user closes the window
//		//depending on the handling of the application window, you might not need this
//		//in this case, if a window is destroyed, we also tell the application to quit entirely by posting a quit message
//
//
//
//	case WM_DESTROY:
//		//sends a quit message to the windows message queue
//		PostQuitMessage(0);
//		return 0;
//
//		/// Key being pressed down events 
//	case WM_KEYDOWN:
//		switch (wParam)
//		{
//		case VK_ESCAPE:  ///VK_ESCAPE is the escape key
//			DestroyWindow(hWnd);  ///Destroy the window. Note! destroying the window is not the same as destroying/closing the application
//			//destroying the window will post a WM_DESTROY message to the message queue which will lead to postquitmessage(0) being called
//			break;
//		}
//	case 'W':
//		//do something when the 'W' key is pressed
//		break;
//
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam); ///let windows handle everything else with default handling 
//
//
//	}
//
//	return 0;
//}
//
//
//HRESULT InitD3D(HWND hWnd)
//{
//	//Create a struct to hold information about the swap chain
//	DXGI_SWAP_CHAIN_DESC scd = {};
//	//fill the swap chain description struct
//	scd.BufferCount = 1;  //one back buffer 
//	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //use 32-bit color 
//	scd.BufferDesc.Width = SCREEN_WIDTH;  //set the back buffer width
//	scd.BufferDesc.Height = SCREEN_HEIGHT;  //set the back buffer height
//	scd.BufferDesc.RefreshRate.Numerator = 60;  // 60 FPS
//	scd.BufferDesc.RefreshRate.Denominator = 1;  // 60/1 = 60 FPS
//	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how the swap chain should be used
//	scd.OutputWindow = hWnd;  // the window to be used
//	scd.SampleDesc.Count = 1;  // how many samples for AA
//	scd.Windowed = TRUE;  // windowed/full-screen mode
//	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // allow full-screen switching
//
//
//	HRESULT hr;
//	//create a device, device context and swap chain using the information in the scd struct
//	hr = D3D11CreateDeviceAndSwapChain(NULL, //use default adapter
//		D3D_DRIVER_TYPE_HARDWARE,            //use hardware acceleration, can also use software or WARP renderers
//		NULL,                                //for software driver type
//		D3D11_CREATE_DEVICE_DEBUG,           // flags can be OR'D together. we are enabling debug for better warnings and errors 
//		NULL,                                //direct3d feature levels. NULL will use d3d11.0 or older.
//		NULL,                                // size of array passed to above member - NULL since we passed NULL
//		D3D11_SDK_VERSION,                   // always set to D3D11_SDK_VERSION 
//		&scd,                               //pointer to our swap chain description
//		&g_swapChain,                          //pointer to our swap chain COM object
//		&g_dev,                              //pointer to our device 
//		NULL,                                //Out param - will be set to chosen feature level 
//		&g_devcon);                          // pointer to our immediate device context
//
//	if (FAILED(hr))
//		return hr;
//
//	return S_OK;
//}
//
//void CleanD3D()
//{
//	if (g_swapChain) g_swapChain->Release();
//	if (g_dev) g_dev->Release();
//	if (g_devcon) g_devcon->Release();
//}
//
//void OpenConsole()
//{
//	if (AllocConsole())
//	{
//		/// console opened successfully 
//
//		//read from console
//		/*wchar_t c[7];
//		memset(c, 0, sizeof(c));
//		DWORD n;
//		ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &c, 5, &n, 0);*/
//		//will have a \r and \n and new line if entire input fit in buffer 
//
//
//		//write to console
//		//wchar_t text[] = L"Hello Console!\n";
//		//DWORD n; // &n can be passed instead of NULL to get how many characters were written 
//		//WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text, wcslen(text), &n, 0); //If you wish to follow this method, I highly suggest creating a wrapper class for handling console IO in a useful manner.
//		/////HINT :::::: CREATE WRAPPER CLASS FOR CONSOLE IO TO MAKE IT EASIER TO USE 
//
//		//THE BELOW REDIRECTS STDIO TO CONSOLE 
//
//		FILE* fp = nullptr;
//		freopen_s(&fp, "CONIN$", "r", stdin);
//		freopen_s(&fp, "CONOUT$", "w", stdout);
//		freopen_s(&fp, "CONOUT$", "w", stderr);
//		std::ios::sync_with_stdio(true);
//
//		std::cout << "Hello Console!" << std::endl;
//	}
//}

