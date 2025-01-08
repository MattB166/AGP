#include "SceneManager.h"
#include <iostream>
#include "../AssetManager/AssetManager.h"
#include "../Time/TimeClass.h"

std::unordered_map<std::wstring, std::unique_ptr<Scene>> SceneManager::m_scenes;
std::wstring SceneManager::m_activeScene;
std::unordered_map<const wchar_t*, std::shared_ptr<SkyBox>> SceneManager::m_LoadedSkyBoxes;
std::unordered_map<std::wstring, std::unique_ptr<Scene>>::iterator SceneManager::m_sceneIterator; 

void SceneManager::AddScene(const std::wstring& name)
{
	if (m_scenes.find(name) != m_scenes.end())
	{
		std::cout << "Scene already exists" << std::endl;
		return;
	}
	auto scene = std::make_unique<Scene>(name);
	m_scenes.insert(std::make_pair(name, std::move(scene)));
	if (m_scenes.size() == 1)
	{
		m_sceneIterator = m_scenes.begin();
		m_activeScene = name;
	}
	std::cout << "Scene Added" << std::endl; 
}

Scene* SceneManager::GetActiveScene()
{
	if (m_scenes.find(m_activeScene) != m_scenes.end())
	{
		return m_scenes[m_activeScene].get();
	}
	return nullptr;
}

std::string SceneManager::GetActiveSceneName()
{
	if (m_activeScene.empty())
	{
		return "";
	}
	auto scene = m_scenes[m_activeScene].get();
	return scene->GetName();
}

void SceneManager::CycleActiveScene()
{
	if (m_scenes.empty())
	{
		return;
	}
	m_sceneIterator++;
	if (m_sceneIterator == m_scenes.end())
	{
		m_sceneIterator = m_scenes.begin();
	}
	m_activeScene = m_sceneIterator->first;
	std::cout << "Active Scene: " << GetActiveSceneName() << std::endl;


}

void SceneManager::InitialiseActiveScene()
{
	if (m_activeScene.empty())
	{
		return;
	}
	auto scene = m_scenes[m_activeScene].get();
	scene->Initialize();
	std::cout << "Scene Initialised" << std::endl;
}

void SceneManager::DeleteCurrentScene()
{
	if (m_scenes.empty() || m_scenes.size() == 1)
	{
		return;
	}
	auto scene = m_scenes[m_activeScene].get();
	m_scenes.erase(m_activeScene);
	m_sceneIterator = m_scenes.begin();

}

void SceneManager::SetActiveScene(const std::wstring& name)
{
	if (m_scenes.find(name) != m_scenes.end())
	{
		m_activeScene = name;
		//std::cout << "Active Scene Set" << std::endl;
	}
}

void SceneManager::AddSkyBoxTextureToActiveScene(const wchar_t* texturePath) //needs amendment as cannot add skyboxes to second scene. 
{
	//check whether the skybox is already loaded, and if the scene already has it.
	if (m_activeScene.empty())
	{
		return;
	}
	auto scene = m_scenes[m_activeScene].get();
	const auto& skyBoxesInScene = scene->GetSkyBoxes();
	for (const auto& skyBox : skyBoxesInScene)
	{
		if (skyBox->GetTexturePath() == texturePath)
		{
			std::cout << "Scene already has skybox" << std::endl;
			return;
		}
	}
	
	std::shared_ptr<SkyBox> skybox;
	if (m_LoadedSkyBoxes.find(texturePath) == m_LoadedSkyBoxes.end())
	{
		skybox = AssetManager::CreateSkyBox(texturePath, "Source/SavedModels/cube.obj", L"CompiledShaders/SkyBoxVShader.cso", L"CompiledShaders/SkyBoxPShader.cso","Sky Box Shader");
		m_LoadedSkyBoxes.insert(std::make_pair(texturePath, skybox));
	}
	else
	{
		std::cout << "Skybox already loaded so giving scene the pre loaded one. " << std::endl;
		skybox = m_LoadedSkyBoxes[texturePath];
	}

	scene->AddSkyBoxToScene(skybox);
	//scene->ChangeActiveSkyBox(skybox);

}

void SceneManager::RemoveSkyBoxFromActiveScene(const wchar_t* texturePath)
{
	if (m_activeScene.empty())
	{
		return;
	}
	auto skybox = m_LoadedSkyBoxes[texturePath];
	auto scene = m_scenes[m_activeScene].get();
	scene->RemoveSkyBoxFromScene(skybox); 
	m_LoadedSkyBoxes.erase(texturePath); 

	
}

void SceneManager::SetActiveSkyBoxTexture(const wchar_t* texturePath)
{
	auto scene = m_scenes[m_activeScene].get();
	//scene->ChangeActiveSkyBox(AssetManager::CreateSkyBox(texturePath, "Source/SavedModels/cube.obj", L"CompiledShaders/SkyBoxVShader.cso", L"CompiledShaders/SkyBoxPShader.cso"));
	//search for the skybox in the loaded skyboxes map.
	auto skybox = m_LoadedSkyBoxes[texturePath];
	if (skybox)
	{
		scene->ChangeActiveSkyBox(skybox);
	}
}

void SceneManager::AddGameObjectToActiveScene(std::unique_ptr<GameObject> obj)
{
	auto scene = m_scenes[m_activeScene].get();
	scene->AddGameObject(std::move(obj));
	std::cout << "Gameobject Added to scene" << GetActiveSceneName() << std::endl;
	//show the gameobject in the scene manager.
}

void SceneManager::CycleGameObjectsInActiveScene()
{
	auto scene = m_scenes[m_activeScene].get();
	scene->CycleSelectedGameObject();
	//show the selected gameobject in the scene manager.
}

GameObject* SceneManager::GetSelectedGameObjectInActiveScene()
{
	auto scene = m_scenes[m_activeScene].get();
	return scene->GetSelectedGameObject();
}

const char* SceneManager::GetSelectedGameObjectNameInActiveScene()
{
	auto go = GetSelectedGameObjectInActiveScene();
	if (go)
	{
		return go->GetName();
	}
	return "";
}

void SceneManager::AddComponentToSelectedGameObjectInActiveScene(std::shared_ptr<Component> comp)
{
	auto scene = m_scenes[m_activeScene].get();
	auto go = scene->GetSelectedGameObject();
	if (go)
	{
		go->AddComponent(comp);
	}
	//show the component in the scene manager.
}

void SceneManager::DisplayActiveObjectDebugWindow()
{
	auto go = GetSelectedGameObjectInActiveScene();
	if (go)
	{
		go->ShowComponentDebugWindow();
	}
	//show the debug window in the scene manager.
}

void SceneManager::CycleSceneSkyBox()
{
	auto scene = m_scenes[m_activeScene].get();
	scene->CycleThroughSkyBoxes();
	//show size of skyboxes in scene manager.
	

}

void SceneManager::DrawScenePreview() 
{
	if (m_activeScene.empty())
	{
		return;
	}
	auto scene = m_scenes[m_activeScene].get();
	scene->DrawStatics(); //draws the static view of the current scene. 

}

void SceneManager::DrawScene()
{
}

void SceneManager::MoveActiveSceneCamera(float x, float y, float z)
{
	auto scene = m_scenes[m_activeScene].get();
	
	if (scene)
	{
		scene->GetCamera()->MoveCamera(x, y, z);
	}
	
}

void SceneManager::RotateActiveSceneCamera(float pitch, float yaw)
{
	auto scene = m_scenes[m_activeScene].get();
	if (scene)
	{
		scene->GetCamera()->RotateCamera(pitch, yaw);
	}
}

void SceneManager::SetActiveSceneCameraTarget(float x, float y, float z, bool TargetMode)
{
	auto scene = m_scenes[m_activeScene].get();
	if (scene)
	{
		scene->GetCamera()->SetTargetPos(x, y, z);
		scene->GetCamera()->SetObjectFocusView(TargetMode);
	}


}

void SceneManager::ResetActiveSceneCamera()
{
	auto scene = m_scenes[m_activeScene].get();
	if (scene)
	{
		scene->GetCamera()->Initialise();
	}

}

void SceneManager::ResetActiveObjectPosition()
{
	auto go = GetSelectedGameObjectInActiveScene();
	if (go)
	{
		go->Initialise();
	}
}

void SceneManager::CleanUp()
{
	m_scenes.clear();
	m_sceneIterator = m_scenes.end();
	m_activeScene.clear();
}
