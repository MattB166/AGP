#include "Window.h"
#include <iostream>
Renderer* Window::s_renderer = nullptr;
Window::Window()
{
	
}

HRESULT Window::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;  ///store the instance handle in our global variable
	///First we need to register a window class
	//This is how windows stores properties of a window that we want to create
	WNDCLASSEX wc = { };  ///initialize all members of the struct to 0. Can also be achieved with "ZeroMemory(&wc, sizeof(WNDCLASSEX))" macro "
	///Fill in the struct with the necessary information
	wc.cbSize = sizeof(WNDCLASSEX);  ///size of the struct
	wc.style = CS_HREDRAW | CS_VREDRAW;  ///style of the window class
	wc.lpfnWndProc = WndProc;  ///Our window procedure function, make sure it handles window creation or calls DefWindowProc(..)
	wc.hInstance = hInstance;  ///Give our app's handle 
	wc.hIcon = LoadIcon(NULL, IDI_SHIELD);  ///Load the icon for the window. IDI_APPLICATION is a default icon that windows provides
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);  ///Load the different types of cursors that windows provides. IDC_CROSS is a crosshair cursor 
	wc.lpszClassName = L"WindowClass1";  ///Windows will store our window class with this name
	wc.hbrBackground = (HBRUSH)(COLOR_HIGHLIGHT);  ///Background color of the win32 app. (not needed for D3D apps)
	//register class with above struct. if it fails, if block will execute

	///ADJUSTING THE WINDOW DIMENSIONS SO THAT THE TOP WINDOWS BAR IS NOT TAKING PIXELS AWAY FROM OUR APP
	 wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	GetClientRect(hWnd, &wr);  ///Get the client area of the window that was set previously 
	UINT width = wr.right - wr.left;
	UINT height = wr.bottom - wr.top;

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);  ///Adjust the window size to fit the desired client area size

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return E_FAIL;
	}


	///Create the window and use the result as the handle to the window
	hWnd = CreateWindowEx(NULL,
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
		hInstance, // application handle
		NULL); // used with multiple windows, NULL

	if (hWnd == NULL) return E_FAIL;

	ShowWindow(hWnd, nCmdShow);  ///Show the window
	cursorPos = { 0, 0 };
	return S_OK;
}

void Window::Run(Renderer* renderer)
{
	s_renderer = renderer; 
	MSG msg; 
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
			HandleInput(TestTime::getDeltaTime());
			renderer->RenderFrame();
		}
		//TimeClass::CalculateFrameStats(TimeClass::GetDeltaTime());
		//TrackCursor();
		//LockCursorToWindow();
		//TimeClass::Tick();
		TestTime::CalculateFrameStats();
		TestTime::Tick();
		//std::cout << TimeClass::GetDeltaTime() << std::endl;
		

	}

}

void Window::OpenConsole()
{
	if (AllocConsole())
	{
		/// console opened successfully 

		//read from console
		/*wchar_t c[7];
		memset(c, 0, sizeof(c));
		DWORD n;
		ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &c, 5, &n, 0);*/
		//will have a \r and \n and new line if entire input fit in buffer 


		//write to console
		//wchar_t text[] = L"Hello Console!\n";
		//DWORD n; // &n can be passed instead of NULL to get how many characters were written 
		//WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text, wcslen(text), &n, 0); //If you wish to follow this method, I highly suggest creating a wrapper class for handling console IO in a useful manner.
		/////HINT :::::: CREATE WRAPPER CLASS FOR CONSOLE IO TO MAKE IT EASIER TO USE 

		//THE BELOW REDIRECTS STDIO TO CONSOLE 

		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);

		std::cout << "Hello Console!" << std::endl;
		

	}
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	float change = XM_PI / 8;
	switch (message)
	{
		//This message is sent when the user closes the window
		//depending on the handling of the application window, you might not need this
		//in this case, if a window is destroyed, we also tell the application to quit entirely by posting a quit message

	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
	case WM_INPUT:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			//this is where you would handle the classic alt+enter fullscreen toggle 
		}
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_DESTROY:
		//sends a quit message to the windows message queue
		PostQuitMessage(0);
		return 0;

		/// Key being pressed down events 
	
		default:
		return DefWindowProc(hWnd, message, wParam, lParam); ///let windows handle everything else with default handling 
		break;
		}
		

	return 0;
}

void Window::TrackCursor()
{
	GetCursorPos(&cursorPos); ///tracks cursor position
	ScreenToClient(hWnd, &cursorPos); ///converts cursor position to client area position, so 0,0 is top left of client window 
	std::cout << "X: " << cursorPos.x << " Y: " << cursorPos.y << std::endl;
	


}

void Window::LockCursorToWindow()
{
	//lock cursor to window
	GetClientRect(hWnd, &wr);
	if (cursorPos.x < wr.left - 50|| cursorPos.x > wr.right + 50|| cursorPos.y < wr.top - 50|| cursorPos.y > wr.bottom + 50)
	{
		POINT p = { (wr.right - wr.left) / 2, (wr.bottom - wr.top) / 2 };
		ClientToScreen(hWnd, &p);
		SetCursorPos(p.x,p.y); //reset to last acceptable position instead of centre of window
	}
}

void Window::HandleInput(float deltaTime)
{
	auto kbState = keyboard.GetState();
	tracker.Update(kbState);
	if (kbState.Escape)
	{
		PostQuitMessage(0);
	}

	if (kbState.W)
	{
        s_renderer->MoveCamera(0.0f, 0.0f, 1.0f * TestTime::getDeltaTime());
	}
	if (kbState.S) ////very smooth movement 
	{
		s_renderer->MoveCamera(0.0f, 0.0f, -1.0f * TestTime::getDeltaTime());
	}
	if (tracker.pressed.T)
	{
		TestTime::SetTimeScale(3.0f);
	}
	if (tracker.released.T)
	{
		TestTime::SetTimeScale(1.0f);
	}
	if (tracker.pressed.C)
	{
		TestTime::SetTimeScale(0.25f);
	}
	if (tracker.released.C)
	{
		TestTime::SetTimeScale(1.0f);
	}

	if (tracker.IsKeyPressed(DirectX::Keyboard::Keys::A)) //requires constant key pressing 
	{
		s_renderer->MoveCamera(-0.001f, 0.0f, 0.0f);
	}


}
