#include "Application.h"
#include <memory>
#include <iostream>
#include "../Input/KeyboardMouse.h"
#include "../AssetManager/AssetManager.h"
#include "../Time/TimeClass.h"

Application::Application() : m_window(nullptr), m_renderer(nullptr), m_inputManager(std::make_unique<KeyboardMouse>())
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

	

	if (FAILED(m_window->InitWindow(hInstance, nCmdShow)))
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
	SceneManager::AddScene(L"Scene2");
	SceneManager::SetActiveScene(L"Scene1");
	SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox01.dds");
	SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox02.dds");
	SceneManager::SetActiveSkyBoxTexture(L"Source/SavedSkyBoxTextures/skybox01.dds");

	while (WM_QUIT != msg.message)
	{
		//m_window->Run();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			///Run game code here
			HandleInput();
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

			m_renderer->Present();
		}
		
		TimeClass::Tick();
		//std::cout << "Delta Time: " << TimeClass::GetDeltaTime() << std::endl; 
		//TimeClass::CalculateFrameStats();
		
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


		ImGui::Begin("Scene Editor", nullptr);
		//want a way to call a function when button is pressed.
		if (ImGui::Button("Cycle SkyBox"))
		{
			SceneManager::CycleSceneSkyBox();
		}
		ImGui::End();

		//std::cout << "Active Scene: " << SceneManager::GetActiveSceneName() << std::endl;


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
	if (m_inputManager != nullptr)
	{
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::P, BindingData(std::bind(&Application::SwitchMode, this), KeyState::Pressed));
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::C, BindingData([]() {SceneManager::CycleSceneSkyBox(); }, KeyState::Pressed));
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::Q, BindingData([]() {SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox01.dds"); }, KeyState::Pressed));
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::A, BindingData([]() {SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox02.dds"); }, KeyState::Pressed));
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::O, BindingData([]() {SceneManager::RemoveSkyBoxFromActiveScene(L"Source/SavedSkyBoxTextures/skybox01.dds"); }, KeyState::Pressed));
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::R, BindingData([]() {SceneManager::RemoveSkyBoxFromActiveScene(L"Source/SavedSkyBoxTextures/skybox02.dds"); }, KeyState::Pressed));
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::S, BindingData([]() {SceneManager::CycleActiveScene(); }, KeyState::Pressed));
		m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::K, BindingData([]() {SceneManager::RotateActiveSceneCamera(0.0f, 2.0f * TimeClass::GetDeltaTime()); }, KeyState::Held)); //little test, need another way to rotate the camera by using mouse. 
	}
	else
	{
		std::cout << "Input is null" << std::endl;
	}
	
}

void Application::HandleInput()
{
	auto kbState = keyboard.GetState();
	auto mouseState = mouse.GetState();

	m_inputManager->ProcessKeyboardInput(kbState);
	m_inputManager->ProcessMouseInput(mouseState);
}
