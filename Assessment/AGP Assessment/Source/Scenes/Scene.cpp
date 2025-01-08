#include "Scene.h"
#include "../AssetManager/AssetManager.h"
#include <iostream>

Scene::Scene(const std::wstring name) : m_name(name)
{
	//initialize the camera.
	m_ActiveCamera = new Camera();
}

Scene::~Scene()
{
	if (m_ActiveCamera)
	{
		delete m_ActiveCamera;
		m_ActiveCamera = nullptr;
	}
	for (auto& cam : m_cameras)
	{
		delete cam;
		cam = nullptr;
	}
	m_cameras.clear();
	//clear skyboxes
	m_skyBoxes.clear();
	//clear gameobjects
	m_gameObjects.clear();
}

void Scene::Initialize()
{
	//for each gameobject in the scene, initialize it back to its starting position in the scene. 
	for (const auto& go : m_gameObjects)
	{
		go->Initialise();
	}

}

void Scene::Update()
{
	/*if (m_deletionQueue.size() > 0)
	{
		m_deletionQueue.clear();
		std::cout << "Deletion queue size: " << m_deletionQueue.size() << std::endl;
		std::cout << "Game Objects in Scene: " << m_gameObjects.size() << std::endl;

	}*/
}

void Scene::ChangeActiveSkyBox(std::shared_ptr<SkyBox> sb)
{
	//if the skybox is not null, set it to the active skybox.
	if (std::find(m_skyBoxes.begin(), m_skyBoxes.end(), sb) != m_skyBoxes.end())
	{
		m_ActiveSkyBox = sb;
	}
	
}

void Scene::AddSkyBoxToScene(std::shared_ptr<SkyBox> sb)
{
	m_skyBoxes.push_back(sb);
	//if there is only one skybox, set it to the active skybox.
	if (m_skyBoxes.size() == 1)
	{
		//std::cout << "Changing Active Skybox as the scene only has one." << std::endl;
		ChangeActiveSkyBox(sb);
	}
}

void Scene::RemoveSkyBoxFromScene(std::shared_ptr<SkyBox> sb)
{
	//if there are no skyboxes, return.
	if (m_skyBoxes.empty() || m_skyBoxes.size() < 0)
	{
		return;
	}
	//find the skybox in the vector.
	auto it = std::find(m_skyBoxes.begin(), m_skyBoxes.end(), sb);
	//if the skybox is found, remove it.
	if (it != m_skyBoxes.end())
	{
		if (m_ActiveSkyBox == sb)
		{
			CycleThroughSkyBoxes();
		}
		m_skyBoxes.erase(it);
		//std::cout << "Skybox Removed" << std::endl;
	}
}

void Scene::AddGameObject(std::unique_ptr<GameObject> obj)
{
	//add the gameobject to the scene.
	static int count = 0;
	std::string BaseName = obj->GetName();
	std::string uniqueName = BaseName + std::to_string(count);

	obj->SetName(uniqueName.c_str());

	m_gameObjects.push_back(std::move(obj));
	count++;
	//if there is only one gameobject, set it to the selected gameobject.
	if (m_gameObjects.size() == 1)
	{
		m_selectedGameObject = m_gameObjects[0].get();
		//std::cout << "Selected Gameobject Changed" << std::endl;
	}
}

void Scene::RemoveActiveGameObject()
{
	//add the object to deletion queue and remove from the scene.
	if (m_selectedGameObject)
	{
		//m_deletionQueue.push_back(std::move(m_gameObjects[m_selectedObjectIndex]));
		m_gameObjects.erase(m_gameObjects.begin() + m_selectedObjectIndex);
		//change active gameobject to the next one in the list.
		CycleSelectedGameObject();
	}

}

void Scene::CycleThroughSkyBoxes()
{
	//if there are no skyboxes, return.
	if (m_skyBoxes.empty() || m_skyBoxes.size() == 1)
	{
		return;
	}
	m_skyBoxIndex = (m_skyBoxIndex + 1) % m_skyBoxes.size();
	ChangeActiveSkyBox(m_skyBoxes[m_skyBoxIndex]);
	//std::cout << "Skybox Changed" << std::endl;

}

void Scene::DrawStatics()
{
	//DisplaySceneDebugWindow();
	//if the skybox is not null, draw it.
	if (m_ActiveSkyBox && m_ActiveCamera)
	{
		//std::cout << "Drawing Skybox" << std::endl;
		m_ActiveSkyBox->Draw(m_ActiveCamera);
	}
	XMMATRIX view = m_ActiveCamera->GetViewMatrix();
	XMMATRIX proj = m_ActiveCamera->GetProjectionMatrix();
	
	//for each gameobject in the scene, draw it and the skybox.
	for (const auto& go : m_gameObjects)
	{
		go->Draw(view, proj);
	}
}

void Scene::CycleSelectedGameObject()
{
	//if there are no gameobjects, return.
	if (m_gameObjects.empty())
	{
		return;
	}
	//cycle through the gameobjects.
	m_selectedObjectIndex = (m_selectedObjectIndex + 1) % m_gameObjects.size();
	m_selectedGameObject = m_gameObjects[m_selectedObjectIndex].get();
	//std::cout << "Selected Gameobject Changed to: " << m_selectedGameObject->GetName() << std::endl;


}

void Scene::MoveActiveCamera(float x, float y, float z)
{
	//if the camera is not null, move it.
	if (m_ActiveCamera)
	{
		m_ActiveCamera->MoveCamera(x, y, z);
	}
}

void Scene::RotateActiveCamera(float pitch, float yaw)
{
	//if the camera is not null, rotate it.
	if (m_ActiveCamera)
	{
		m_ActiveCamera->RotateCamera(pitch, yaw);
	}
}

void Scene::AddCamera()
{
	Camera* newCam = new Camera();
	m_cameras.push_back(newCam);
	std::cout << "Cameras size: " << m_cameras.size() << std::endl;
}

void Scene::DisplaySceneDebugWindow()
{
	ImGui::Begin("Scene Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
	ImGui::Text("Current Scene: %s", std::string(m_name.begin(), m_name.end()).c_str());
	
	
	if (ImGui::Button("Cycle SkyBox"))
	{
		CycleThroughSkyBoxes();
	}
	if (GetObjectCount() > 1)
	{
		if (ImGui::Button("Cycle Object"))
		{
			CycleSelectedGameObject();
		}
	}
	
	if (ImGui::Button("Add GameObject"))
	{
		ImGui::OpenPopup("New GameObject");
	}
	if (GetObjectCount() > 0)
	{
		if (ImGui::Button("Remove GameObject"))
		{
			RemoveActiveGameObject();
		}
	}
	if (ImGui::BeginPopupModal("New GameObject"))
	{

		static char name[128] = "GameObject";
		static bool isReflected = false;
		ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
		ImGui::Checkbox("Reflective", &isReflected);
		if (ImGui::Button("Create", ImVec2(120, 0)))
		{
			auto go = std::make_unique<GameObject>(name, isReflected);
			AddGameObject(std::move(go));

			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	
	ImGui::SetWindowPos(ImVec2(0, 100));
	ImGui::SetWindowSize(ImVec2(200, 200));
	ImGui::End();


	
}
