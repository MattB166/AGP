#pragma once
#include <Windows.h>
#include <memory>
#include "../Input/IInputManager.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
class Application;
class Window
{
public:
	Window();
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow, std::unique_ptr<IInputManager>& inp);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetHWND() { return m_hwnd; }
	void Run(); //probably not needed 
	void OpenConsole();
	IInputManager* GetInput() { return m_Input.get(); }

private:
	HINSTANCE m_hInstance = NULL;
	HWND m_hwnd = NULL; 
	RECT wr;
	std::unique_ptr<IInputManager> m_Input; 
};

