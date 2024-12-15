#include "Window.h"
#include <iostream>
#include "../IMGUI/imgui.h"
#include "../IMGUI/imgui_impl_win32.h"
#include "../IMGUI/imgui_impl_dx11.h"
IInputManager* Window::m_InputStatic = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window() : m_hInstance(NULL), m_hwnd(NULL), wr{0,0,0,0}
{
}

HRESULT Window::InitWindow(HINSTANCE hInstance, int nCmdShow, IInputManager* inp)
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
	m_Input = inp;
	m_InputStatic = inp;
	

	return S_OK;

}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	if (m_InputStatic != nullptr)
		m_InputStatic->ProcessInput(hWnd, message, wParam, lParam); //process input
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
	m_InputStatic->Update();
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
