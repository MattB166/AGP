#include <Windows.h>


	////global variables
	HINSTANCE g_hInst = NULL;   // Handle to this instance
	HWND g_hWnd = NULL;         // Handle to our created window
	const wchar_t* windowName = L"Week 1 Window Creation"; // Name of our window


	////FORWARD DECLARATIONS FOR FUNCTION PROTOTYPES
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) ///returns an int value. OS doesnt use the return value but can use this
///value to pass a status code onto another program that is waiting for the completion of this program 
{

	if (FAILED(InitWindow(hInstance, nCmdShow)))  ///If the window initialization fails, return an error code
	{
		MessageBox(NULL, L"Window Initialization Failed!", L"Error!", MB_ICONERROR | MB_OK);
	}
	MSG msg;  ///Windows message struct 

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  ///Check if there is a message in the message queue
		{
			//translate certain messages into correct format, namely key presses
			TranslateMessage(&msg);

			//send the message to the window procedure
			DispatchMessage(&msg);

			//break out of the loop if the message is WM_QUIT
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			//run game code here
			//this is where you would put your game loop
		}
			
		
	}



	return 0;
}
///hInstance: Handle to the current instance of the application. OS uses this to identify the executable when it is loaded in memory
///hPrevInstance: Handle to the previous instance of the application - has no meaning in Win32 programming and is always zero 
/// lpCmdLine: Command line arguments as a Unicode string
/// nCmdShow: How the window is to be shown. i.e. maximized, minimized, etc.

HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow)
{
	g_hInst = instanceHandle;  ///store the instance handle in our global variable
	///First we need to register a window class
	//This is how windows stores properties of a window that we want to create
	WNDCLASSEX wc = { };  ///initialize all members of the struct to 0. Can also be achieved with "ZeroMemory(&wc, sizeof(WNDCLASSEX))" macro "
	///Fill in the struct with the necessary information
	wc.cbSize = sizeof(WNDCLASSEX);  ///size of the struct
	wc.style = CS_HREDRAW | CS_VREDRAW;  ///style of the window class
	wc.lpfnWndProc = WndProc;  ///Our window procedure function, make sure it handles window creation or calls DefWindowProc(..)
	wc.hInstance = instanceHandle;  ///Give our app's handle 
	wc.lpszClassName = L"WindowClass1";  ///Windows will store our window class with this name
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);  ///Background color of the win32 app. (not needed for D3D apps)
	//register class with above struct. if it fails, if block will execute

	///ADJUSTING THE WINDOW DIMENSIONS SO THAT THE TOP WINDOWS BAR IS NOT TAKING PIXELS AWAY FROM OUR APP
	RECT wr = { 0, 0, 640, 480 };   
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);  ///Adjust the window size to fit the desired client area size

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return E_FAIL;
	}


	///Create the window and use the result as the handle to the window
	g_hWnd = CreateWindowEx(NULL,
		L"WindowClass1", // name of the window class
		windowName, // title of the window
		WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU, // window style with no resizing and maximizing 
		//WS_OVERLAPPEDWINDOW, // window style with resizing and maximizing
		100, // x-position of the window
		100, // y-position of the window
		wr.right - wr.left, // width of the window
		wr.bottom - wr.top, // height of the window
		NULL, // we have no parent window, NULL
		NULL, // we aren't using menus, NULL
		instanceHandle, // application handle
		NULL); // used with multiple windows, NULL

	if (g_hWnd == NULL) return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);  ///Show the window
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//This message is sent when the user closes the window
		//depending on the handling of the application window, you might not need this
		//in this case, if a window is destroyed, we also tell the application to quit entirely by posting a quit message



	case WM_DESTROY:
		//sends a quit message to the windows message queue
		PostQuitMessage(0);
		return 0;

		/// Key being pressed down events 
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:  ///VK_ESCAPE is the escape key
			DestroyWindow(hWnd);  ///Destroy the window. Note! destroying the window is not the same as destroying/closing the application
			//destroying the window will post a WM_DESTROY message to the message queue which will lead to postquitmessage(0) being called
			break;
		}
	case 'W':
		//do something when the 'W' key is pressed
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam); ///let windows handle everything else with default handling 


	}

	return 0;
}

