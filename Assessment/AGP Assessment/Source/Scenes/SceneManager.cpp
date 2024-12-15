#include "SceneManager.h"

std::unordered_map<const wchar_t*, std::shared_ptr<Scene>> SceneManager::m_scenes;
std::shared_ptr<Scene> SceneManager::m_activeScene;

void SceneManager::AddScene(const wchar_t* name, std::shared_ptr<Scene> sc)
{
	m_scenes.insert(std::make_pair(name, sc));
}

std::shared_ptr<Scene> SceneManager::GetActiveScene()
{
	return m_activeScene;
}
