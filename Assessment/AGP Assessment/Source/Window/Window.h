#pragma once
#include <Windows.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
class Window
{
public:
	Window();
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetHWND() { return m_hwnd; }
	void Run(); //probably not needed 
	void OpenConsole();

private:
	HINSTANCE m_hInstance = NULL;
	HWND m_hwnd = NULL; 
	RECT wr;
};

