#include "Window.h"

Window::Window() : m_hInstance(NULL), m_hwnd(NULL), wr{0,0,0,0}
{
}

HRESULT Window::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	m_hInstance = hInstance;  
	WNDCLASSEX wc = { };  
	
	wc.cbSize = sizeof(WNDCLASSEX);  
	wc.style = CS_HREDRAW | CS_VREDRAW;  
	wc.lpfnWndProc = WndProc; 
	wc.hInstance = hInstance;  
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.lpszClassName = L"WindowClass1";  
	wc.hbrBackground = (HBRUSH)(COLOR_MENU);  

	
	wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	GetClientRect(m_hwnd, &wr);  
	UINT width = wr.right - wr.left;
	UINT height = wr.bottom - wr.top;

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);  

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return E_FAIL;
	}


	
	m_hwnd = CreateWindowEx(NULL,
		L"WindowClass1", 
		L"Project", 
		WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU, 
		100, 
		100, 
		wr.right - wr.left,
		wr.bottom - wr.top, 
		NULL, 
		NULL, 
		hInstance, 
		NULL); 

	if (m_hwnd == NULL) return E_FAIL;

	ShowWindow(m_hwnd, nCmdShow);  
	//cursorPos = { 0, 0 };

	//mouse.SetWindow(hWnd);
	//mouse.SetMode(Mouse::MODE_RELATIVE);
	return S_OK;

}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//float change = XM_PI / 8;
	switch (message)
	{
		//This message is sent when the user closes the window
		//depending on the handling of the application window, you might not need this
		//in this case, if a window is destroyed, we also tell the application to quit entirely by posting a quit message

	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
	case WM_INPUT:
		//Keyboard::ProcessMessage(message, wParam, lParam);
		//Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_MOUSEACTIVATE:
		return MA_ACTIVATEANDEAT;

	case WM_MOUSEMOVE:
		//s_renderer->RotateCube(0.01f,0.0f,0.0f);

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		//Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			//this is where you would handle the classic alt+enter fullscreen toggle 
		}
		//Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		//Keyboard::ProcessMessage(message, wParam, lParam);
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

void Window::Run()
{
	MSG msg = { 0 };  ///initialize the message structure to 0
	///Enter the infinite message loop
	while (WM_QUIT != msg.message)  ///while the message is not a quit message
	{
		///Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  ///if there is a message in the queue
		{
			///Translate the message and dispatch it to the window procedure
			TranslateMessage(&msg);  ///translate the message
			DispatchMessage(&msg);  ///dispatch the message to the window procedure
		}
		else
		{
			///Run game code here
		}
	}
}
