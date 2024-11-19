#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>
#include "../Time/TestTime.h"
#include "../Renderer/Renderer.h"
#pragma once
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
class Window
{
	
public:
	Window();
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	void Run(Renderer* renderer);
	void OpenConsole();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	inline HWND GetWindow() const {return hWnd; }
	void TrackCursor();
	void LockCursorToWindow();
	void HandleInput(float deltaTime);
private:
	HINSTANCE hInst = NULL;   // Handle to this instance
	HWND hWnd = NULL;         // Handle to our created window
	const wchar_t* windowName = L"AGP TUTORIALS"; // Name of our window
	static Renderer* s_renderer;
	POINT cursorPos;
	POINT lastAcceptableCursorPos;
	RECT wr;
	Keyboard keyboard; //temporary - will make an input class later 
	Keyboard::KeyboardStateTracker tracker;
	Mouse mouse;
	Mouse::ButtonStateTracker mouseTracker;


};

