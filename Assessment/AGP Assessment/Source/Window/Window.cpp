#include "Window.h"
#include <iostream>
#include "../IMGUI/imgui.h"
#include "../IMGUI/imgui_impl_win32.h"
#include "../IMGUI/imgui_impl_dx11.h"
//IInputManager* Window::m_InputStatic = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window() : m_hInstance(NULL), m_hwnd(NULL), wr{0,0,0,0}
{
}

Window::~Window()
{
	if (m_hwnd)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;
	}
	if (m_hInstance)
	{
		UnregisterClass(L"WindowClass1", m_hInstance);
		m_hInstance = NULL;
	}
	//if (m_Input)
	//{
	//	m_Input = nullptr; //only setting to null because application will delete the input manager, as it owns it. 
	//}
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
	//m_Input = inp;
	//m_InputStatic = inp;
	return S_OK;
	

	return S_OK;

}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	//float change = XM_PI / 8;
	switch (message)
	{
		//This message is sent when the user closes the window
		//depending on the handling of the application window, you might not need this
		//in this case, if a window is destroyed, we also tell the application to quit entirely by posting a quit message

	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
	case WM_INPUT:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
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
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break; 

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			//this is where you would handle the classic alt+enter fullscreen toggle 
		}
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
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
	//m_InputStatic->Update();
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
