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
	~Window();
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow, IInputManager* inp);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetHWND() { return m_hwnd; }
	void Run(); //probably not needed 
	void OpenConsole();
	IInputManager* GetInput() { return m_Input; }

private:
	HINSTANCE m_hInstance = NULL;
	HWND m_hwnd = NULL; 
	RECT wr;
	IInputManager* m_Input = nullptr;
	static IInputManager* m_InputStatic;

};

