#pragma once
#include <Windows.h>
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include "../IMGUI/imgui.h"
#include "../IMGUI/imgui_impl_win32.h"
#include "../IMGUI/imgui_impl_dx11.h"
#include "../Scenes/SceneManager.h"
#include "../Input/KeyboardMouse.h"
enum class Mode
{
	EDIT,
	PLAY
};

class Application
{
public:
	Application(); 
	~Application(); 

	bool Initialize(HINSTANCE hInstance, int nCmdShow); //initialize window, renderer, imgui. 
	void Run();  //run the application if initialized.
	void SetMode(Mode mode);
	void SwitchMode();
	Mode GetMode() { return m_mode; }
	void RunMode(); 
	void ChangeMouseMode(); 
	void SetupConstantBindings();
	void SetupModeBindings();
	void SetupEditBindings();
	void SetupPlayBindings();
	void HandleInput();



private:
	Window* m_window = nullptr;
	Renderer* m_renderer = nullptr; 
	std::unique_ptr<KeyboardMouse> m_inputManager = nullptr;
	DirectX::Keyboard keyboard;
	DirectX::Mouse mouse;
	Mode m_mode = Mode::EDIT;
	Mouse::Mode m_mouseMode = Mouse::MODE_ABSOLUTE; 
	//list/vector of scenes. 

	//need a way to determine whether we are in "edit" mode or "play" mode. 
};

