#include "SceneManager.h"
#include <iostream>
#include "../AssetManager/AssetManager.h"

std::unordered_map<std::wstring, std::unique_ptr<Scene>> SceneManager::m_scenes;
std::wstring SceneManager::m_activeScene;
std::unordered_map<const wchar_t*, std::shared_ptr<SkyBox>> SceneManager::m_LoadedSkyBoxes;

void SceneManager::AddScene(const std::wstring& name)
{
	auto scene = std::make_unique<Scene>();
	m_scenes.insert(std::make_pair(name, std::move(scene)));
	
}

Scene* SceneManager::GetActiveScene()
{
	if (m_scenes.find(m_activeScene) != m_scenes.end())
	{
		return m_scenes[m_activeScene].get();
	}
	return nullptr;
}

void SceneManager::SetActiveScene(const std::wstring& name)
{
	if (m_scenes.find(name) != m_scenes.end())
	{
		m_activeScene = name;
		std::cout << "Active Scene Set" << std::endl;
	}
}

void SceneManager::AddSkyBoxTextureToActiveScene(const wchar_t* texturePath)
{
	if (m_activeScene.empty())
	{
		return;
	}
	auto scene = m_scenes[m_activeScene].get();
	std::shared_ptr<SkyBox> skyBox = AssetManager::CreateSkyBox(texturePath, "Source/SavedModels/cube.obj", L"CompiledShaders/SkyBoxVShader.cso", L"CompiledShaders/SkyBoxPShader.cso");
	scene->AddSkyBoxToScene(skyBox);
	m_LoadedSkyBoxes.insert(std::make_pair(texturePath, skyBox));



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
	scene->ChangeActiveSkyBox(AssetManager::CreateSkyBox(texturePath, "Source/SavedModels/cube.obj", L"CompiledShaders/SkyBoxVShader.cso", L"CompiledShaders/SkyBoxPShader.cso"));
}

void SceneManager::CycleSceneSkyBox()
{
	auto scene = m_scenes[m_activeScene].get();
	scene->CycleThroughSkyBoxes();

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

void SceneManager::CleanUp()
{
	m_scenes.clear();
	m_activeScene.clear();
}
