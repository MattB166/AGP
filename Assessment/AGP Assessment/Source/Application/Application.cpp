#include "Application.h"
#include <memory>
#include <iostream>
#include "../Input/KeyboardMouse.h"
#include "../AssetManager/AssetManager.h"

Application::Application() : m_window(nullptr), m_renderer(nullptr), m_input(std::make_unique<KeyboardMouse>())
{

	
}

Application::~Application()
{
	if (m_window)
	{
		delete m_window;
		m_window = nullptr;
	}
	if (m_renderer)
	{
		delete m_renderer;
		m_renderer = nullptr;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool Application::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	if (m_window == nullptr)
	m_window = new Window();
	else
	{
		return false;
	}

	

	if (FAILED(m_window->InitWindow(hInstance, nCmdShow, m_input.get())))
	{
		return false;
	}

	if (m_renderer == nullptr)
		m_renderer = new Renderer();
	else
	{
		return false;
	}

	if (FAILED(m_renderer->InitRenderer(m_window->GetHWND(), SCREEN_WIDTH, SCREEN_HEIGHT)))
	{
		return false;
	}

	m_window->OpenConsole();	


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(m_window->GetHWND());
	ImGui_ImplDX11_Init(m_renderer->GetDevice(), m_renderer->GetDeviceContext());
	AssetManager::Initialize(m_renderer->GetDevice(), m_renderer->GetDeviceContext());

	

	return true;
}

void Application::Run()
{
	MSG msg = { 0 };
	SetupBindings();
	SceneManager::AddScene(L"Scene1");
	SceneManager::SetActiveScene(L"Scene1");
	SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox01.dds");

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			///Run game code here
			m_renderer->Clear();

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			RunMode();
			//draw current scene.
			//draw scene preview.
			SceneManager::DrawScenePreview();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			m_renderer->Present(SceneManager::GetActiveScene()->GetCamera()->GetViewMatrix(), SceneManager::GetActiveScene()->GetCamera()->GetProjectionMatrix());
		}

	}
}

void Application::SetMode(Mode mode)
{
	m_mode = mode;
	//if mode is play, hide IMGUI, if mode is edit, show IMGUI.
	//send objects back to their original positions if mode is edit, and update behaviours if mode is play. 
	//do this via the scene manager. 
}

void Application::SwitchMode()
{
	if (m_mode == Mode::EDIT)
	{
		m_mode = Mode::PLAY;
	}
	else if (m_mode == Mode::PLAY)
	{
		m_mode = Mode::EDIT;
	}
	//if mode is play, hide IMGUI, if mode is edit, show IMGUI.
	//send objects back to their original positions if mode is edit, and update behaviours if mode is play. 
	//do this via the scene manager.
}

void Application::RunMode() //also in here run all logic for choosing objects and editing them. 
{
	if (m_mode == Mode::EDIT)
	{
		ImGui::Begin("Edit Mode", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		ImGui::Text("Current Mode: Edit Mode. Press P to Play");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::End();
	}
	else if (m_mode == Mode::PLAY)
	{
		ImGui::Begin("Play Mode", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		ImGui::Text("Current Mode: Play Mode. Press P to Exit Play");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::End();
	}
}

void Application::SetupBindings()
{
	if (m_input != nullptr)
	{
		m_input->BindKeyToFunction('P', BindingData(std::bind(&Application::SwitchMode, this), KeyState::Pressed));
	}
	else
	{
		std::cout << "Input is null" << std::endl;
	}
	
}
