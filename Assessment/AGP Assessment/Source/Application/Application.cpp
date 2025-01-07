#include "Application.h"
#include <memory>
#include <iostream>
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
	AssetManager::Initialize(m_renderer->GetDevice(), m_renderer->GetDeviceContext(),m_renderer->GetCBuffer());

	SkyBox::LoadAllSkyBoxNames("Source/SavedSkyBoxTextures");
	Model::LoadAllModelNames("Source/SavedModels");  
	Material::LoadAllTextureNames("Source/SavedTextures");
	ShaderSet::LoadAllShaderNames("CompiledShaders/StandardShader"); 
	//do the same for textures and shaders.
	//and skyboxes 

	mouse.SetWindow(m_window->GetHWND());
	

	return true;
}

void Application::Run()
{
	MSG msg = { 0 };
	SetupConstantBindings();
	SetupModeBindings();
	
	SceneManager::AddScene(L"Scene 1");
	SceneManager::AddScene(L"Scene 2");
	SceneManager::SetActiveScene(L"Scene1");
	SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox01.dds");
	SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox02.dds");
	SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/CustomSpace.dds");
	SceneManager::SetActiveSkyBoxTexture(L"Source/SavedSkyBoxTextures/CustomSpace.dds");
	//GameObject* go = new GameObject("Test Object");
	//std::make_unique<GameObject>("Test Object");
	/*std::shared_ptr<Model> model = AssetManager::CreateModel("Source/SavedModels/cube.obj","Cube");
	std::shared_ptr<ShaderSet> shaderSet = AssetManager::CreateShaderSet(L"CompiledShaders/VertexShader.cso", L"CompiledShaders/PixelShader.cso","Standard Shader");
	std::shared_ptr<Material> material = AssetManager::CreateMaterial(L"Source/SavedTextures/Box.bmp","Box Texture");
	go->AddComponent(material);
	go->AddComponent(shaderSet);
	go->AddComponent(model);
	SceneManager::AddGameObjectToActiveScene(go);*/

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
			SceneManager::GetActiveScene()->Update();
			SceneManager::DrawScenePreview();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			m_renderer->Present();
		}
		
		TimeClass::Tick();
		//std::cout << "Delta Time: " << TimeClass::GetDeltaTime() << std::endl; 
		//TimeClass::CalculateFrameStats();
		
	}
	/*delete go;
	go = nullptr;*/
}

void Application::SetMode(Mode mode)
{
	m_mode = mode;
	std::string modeString = (m_mode == Mode::EDIT) ? "EDIT" : "PLAY";
	std::cout << "Mode set to: " << modeString << std::endl;
	if (m_mode == Mode::EDIT)
		mouse.SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	else if (m_mode == Mode::PLAY)
		mouse.SetMode(DirectX::Mouse::MODE_RELATIVE);
	else


	SetupModeBindings();
}

void Application::SwitchMode()
{
	SceneManager::ResetActiveSceneCamera();
	if (m_mode == Mode::EDIT)
	{
		SetMode(Mode::PLAY);
		SetupModeBindings();
	}
	else if (m_mode == Mode::PLAY)
	{
		SetMode(Mode::EDIT);
		SetupModeBindings();
	}
	//if mode is play, hide IMGUI, if mode is edit, show IMGUI.
	//send objects back to their original positions if mode is edit, and update behaviours if mode is play. 
	//do this via the scene manager.

	if (followSelectedObject)
	{
		SceneManager::SetActiveSceneCameraTarget(0.0f, 0.0f, 0.0f, false);
		followSelectedObject = false;
	}

}

void Application::RunMode() //also in here run all logic for choosing objects and editing them. 
{
	
	if (m_mode == Mode::EDIT)
	{
		
		ImGui::Begin("Edit Mode", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		ImGui::Text("Current Mode: Edit Mode.");
		ImGui::Text("Current Scene: %s", SceneManager::GetActiveSceneName().c_str());
		if (ImGui::Button("Cycle Scene"))
		{
			SceneManager::CycleActiveScene();
		}
		if (ImGui::Button("Add Skybox"))
		{
			ImGui::OpenPopup("Add SkyBox");
		}
		if (ImGui::Button("Cycle SkyBox"))
		{
			SceneManager::CycleSceneSkyBox();
		}
		if (SceneManager::GetActiveScene()->GetObjectCount() > 1)
		{
			if (ImGui::Button("Cycle Object"))
			{
				SceneManager::CycleGameObjectsInActiveScene();
			}
		}
		if (ImGui::Button("Play"))
		{
			SwitchMode();
		}
		if (ImGui::Button("Add GameObject"))
		{
			ImGui::OpenPopup("New GameObject");
		}
		if (SceneManager::GetActiveScene()->GetObjectCount() > 0)
		{
			if (ImGui::Button("Remove GameObject"))
			{
				SceneManager::GetActiveScene()->RemoveActiveGameObject();
			}
		}
		if (ImGui::BeginPopupModal("New GameObject"))
		{
		
			static char name[128] = "GameObject";
			ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
			if (ImGui::Button("Create", ImVec2(120, 0)))
			{
				auto go = std::make_unique<GameObject>(name);
				SceneManager::AddGameObjectToActiveScene(std::move(go));
				
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Add SkyBox"))
		{
			//get sky box options from skybox class 
			ImGui::Separator();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::End();


		if (SceneManager::GetActiveScene()->GetObjectCount() > 0)
		{
			float x = SceneManager::GetSelectedGameObjectInActiveScene()->GetTransform().pos.x; 
			float y = SceneManager::GetSelectedGameObjectInActiveScene()->GetTransform().pos.y;
			float z = SceneManager::GetSelectedGameObjectInActiveScene()->GetTransform().pos.z;

			ImGui::Begin("Scene Hierarchy", nullptr, ImGuiWindowFlags_NoMove);
			
			
			ImGui::Text("Currently Selected Object:");
			SceneManager::DisplayActiveObjectDebugWindow();
			ImGui::Checkbox("Rotate Camera to View Object", &followSelectedObject);
			if (followSelectedObject)
			{
				SceneManager::SetActiveSceneCameraTarget(x, y, z, true);
			}
			if (ImGui::Button("Reset Position"))
			{
				followSelectedObject = false;
				SceneManager::SetActiveSceneCameraTarget(0.0f, 0.0f, 0.0f, false);
				SceneManager::ResetActiveObjectPosition();
				SceneManager::ResetActiveSceneCamera();
			}
			//combo for adding components of type, and then choice of different options within that type, to the selected object.
           //if object is selected, show the components of that object, and allow for adding new components to that object.
			
			if (ImGui::Button("Add Component"))
			{
				//selectedComponentType = ComponentType::None;
				ImGui::OpenPopup("Add Component");
			}
			if (ImGui::BeginPopupModal("Add Component",nullptr,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
			{
				std::vector<ComponentType> types = Component::GetTypes();
				types.erase(std::remove(types.begin(), types.end(), ComponentType::Shaders), types.end());
				types.erase(std::remove(types.begin(), types.end(), ComponentType::Texture), types.end());
				for (auto type : types)
				{
					std::string typeName = Component::ComponentTypeToString(type);
					if (ImGui::Button(typeName.c_str()))
					{
						selectedComponentType = type;
						std::cout << "Selected Component Type: " << typeName << std::endl;
						showComponentOptions = true;
						
						AddComponent(selectedComponentType);
						ImGui::CloseCurrentPopup();
					}
					

				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					showComponentOptions = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if(showComponentOptions)
			{
				ImGui::OpenPopup("Component Options");
			}
			if (ImGui::BeginPopupModal("Component Options"))
			{
				//show options for the selected component type.
				for (const auto& option : options)
				{
					if (ImGui::Button(option.c_str()))
					{ 
						/////need different way to account for shaders. 
						std::string fPath = component->GetComponentFilePath(option);
						std::shared_ptr<Component> comp = AssetManager::CreateComponentFromFilePath(fPath, selectedComponentType,option.c_str()); ////CURRENTLY FAILS HERE WITH SHADERS 
						SceneManager::GetSelectedGameObjectInActiveScene()->AddComponent(comp);
						showComponentOptions = false;
						ImGui::CloseCurrentPopup();

					}
				}
			
				ImGui::Separator();
				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					showComponentOptions = false;
					//options.clear();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			//ImGui::EndPopup();
			
			
			ImGui::SetWindowPos(ImVec2(0, 180));
			ImGui::End();
		}
		
		


	}
	else if (m_mode == Mode::PLAY)
	{
		ImGui::Begin("Play Mode", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		ImGui::Text("Current Mode: Play Mode. Press ESC to exit Play.");
		ImGui::Text("Current Scene: %s", SceneManager::GetActiveSceneName().c_str());
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::End();

		/*if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			mouse.SetMode(DirectX::Mouse::MODE_ABSOLUTE);
		}
		else
		{
			mouse.SetMode(DirectX::Mouse::MODE_RELATIVE);
		}*/
	}

	
}

void Application::ChangeMouseMode()
{
	if (m_mouseMode == Mouse::MODE_ABSOLUTE)
	{
		m_mouseMode = Mouse::MODE_RELATIVE;
		mouse.SetMode(DirectX::Mouse::MODE_RELATIVE);
		//mouse.SetWindow(m_window->GetHWND());
	}
	else if (m_mouseMode == Mouse::MODE_RELATIVE)
	{
		m_mouseMode = Mouse::MODE_ABSOLUTE;
		mouse.SetMode(DirectX::Mouse::MODE_ABSOLUTE);
		//mouse.SetWindow(NULL);
	}
}

void Application::SetupConstantBindings()
{
	m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::Escape, BindingData(std::bind(&Application::SwitchMode, this), KeyState::Released));
}

void Application::SetupModeBindings()
{
	if (m_inputManager != nullptr)
	{
		//m_inputManager->ClearAllBindings(); NEED TO CLEAR ALL TEMPORARY BINDINGS BETWEEN EACH STATE CHANGE. BUT KEEP THE CONSTANT BINDINGS THROUGHOUT LOOP. 

		if (m_mode == Mode::EDIT)
		{
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::P, BindingData(std::bind(&Application::SwitchMode, this), KeyState::Pressed));
			
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::C, BindingData([]() {SceneManager::CycleSceneSkyBox(); }, KeyState::Pressed));
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::Q, BindingData([]() {SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox01.dds"); }, KeyState::Pressed));
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::A, BindingData([]() {SceneManager::AddSkyBoxTextureToActiveScene(L"Source/SavedSkyBoxTextures/skybox02.dds"); }, KeyState::Pressed));
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::O, BindingData([]() {SceneManager::RemoveSkyBoxFromActiveScene(L"Source/SavedSkyBoxTextures/skybox01.dds"); }, KeyState::Pressed));
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::R, BindingData([]() {SceneManager::RemoveSkyBoxFromActiveScene(L"Source/SavedSkyBoxTextures/skybox02.dds"); }, KeyState::Pressed));
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::S, BindingData([]() {SceneManager::CycleActiveScene(); }, KeyState::Pressed));
			////create editable value for the snap distance. 
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::K, BindingData([]() {SceneManager::RotateActiveSceneCamera(10.0f, 0.0f); }, KeyState::Held));
			//m_inputManager->BindMouseToFunction(MouseButton::Right, BindingData([]() {SceneManager::RotateActiveSceneCamera(10.0f, 0.0f); }, KeyState::Held));
			m_inputManager->ClearMouseMovement();
			//mouse.SetMode(DirectX::Mouse::MODE_ABSOLUTE);
		}
		else if (m_mode == Mode::PLAY)
		{
			m_inputManager->BindMouseMovement([](int x, int y) {SceneManager::RotateActiveSceneCamera(x,y); });
			m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::D, BindingData([]() {SceneManager::MoveActiveSceneCamera(0.1f, 0.0f, 0.0f); }, KeyState::Held));
			m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::A, BindingData([]() {SceneManager::MoveActiveSceneCamera(-0.1f, 0.0f, 0.0f); }, KeyState::Held));
			m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::W, BindingData([]() {SceneManager::MoveActiveSceneCamera(0.0f, 0.0f, 0.1f); }, KeyState::Held));
			m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::S, BindingData([]() {SceneManager::MoveActiveSceneCamera(0.0f, 0.0f, -0.1f); }, KeyState::Held));
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::Escape, BindingData(std::bind(&Application::ChangeMouseMode, this), KeyState::Pressed));
			//mouse.SetMode(DirectX::Mouse::MODE_RELATIVE);
			// Add play mode specific bindings here
			//m_inputManager->BindKeyToFunction(DirectX::Keyboard::Keys::Escape, BindingData(std::bind(&Application::SwitchMode, this), KeyState::Pressed));
			// Add other play mode bindings as needed
		}
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

	//std::cout << "Mouse State: " << mouseState.x << ", " << mouseState.y << std::endl;

	m_inputManager->ProcessKeyboardInput(kbState);
	m_inputManager->ProcessMouseInput(mouseState);
	
}

void Application::AddComponent(ComponentType type)
{
	options.clear();
	component = AssetManager::CreateTemporaryComponentInstance(type);
	if (component)
	{
		options = component->GetComponentOptions(); 
		std::cout << "Options size" << options.size() << std::endl;
		for (const auto& option : options)
		{
			std::cout << option << std::endl;
		}
	}
	else
	{
		std::cout << "Component is null" << std::endl;
	}
		
}
