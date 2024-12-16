#include "SceneManager.h"

std::unordered_map<std::wstring, std::unique_ptr<Scene>> SceneManager::m_scenes;
std::wstring SceneManager::m_activeScene;

void SceneManager::AddScene(const std::wstring& name, Scene* sc)
{
	m_scenes.insert(std::make_pair(name, sc));
}

Scene* SceneManager::GetActiveScene()
{
	if (m_scenes.find(m_activeScene) != m_scenes.end())
	{
		return m_scenes[m_activeScene].get();
	}
	return nullptr;
}

void SceneManager::SetActiveScene(const wchar_t* name)
{
	if (m_scenes.find(name) != m_scenes.end())
	{
		m_activeScene = name;
	}
}

void SceneManager::CleanUp()
{
	m_scenes.clear();
	m_activeScene.clear();
}
