#include "Window.h"
#include <iostream>

Window::Window() : m_hInstance(NULL), m_hwnd(NULL), wr{0,0,0,0}
{
}

HRESULT Window::InitWindow(HINSTANCE hInstance, int nCmdShow, std::unique_ptr<IInputManager>& inp)
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
		this); 

	if (m_hwnd == NULL) return E_FAIL;

	ShowWindow(m_hwnd, nCmdShow);  
	//cursorPos = { 0, 0 };

	//mouse.SetWindow(hWnd);
	//mouse.SetMode(Mouse::MODE_RELATIVE);
	m_Input = std::move(inp);

	

	return S_OK;

}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	/*Window* wThis = nullptr;
	if (message == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		wThis = (Window*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wThis);
		wThis->m_hwnd = hWnd;
		std::cout << "Window pointer set" << std::endl;
	}
	else
	{
		wThis = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (wThis == nullptr)
		{
			std::cout << "Window pointer is null" << std::endl;
		}
	}

	if (wThis)
	{
		wThis->m_Input->ProcessInput(hWnd, message, wParam, lParam);
	}*/
	switch (message)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam); ///let windows handle everything else with default handling 
		
}

void Window::Run()
{
	
	
}

void Window::OpenConsole()
{
	if (AllocConsole())
	{
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);

		std::cout << "Hello Console!" << std::endl;

	}
}
