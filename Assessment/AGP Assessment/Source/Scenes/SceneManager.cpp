#include "SceneManager.h"

std::unordered_map<std::wstring, std::unique_ptr<Scene>> SceneManager::m_scenes;
std::wstring SceneManager::m_activeScene;

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
	}
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
