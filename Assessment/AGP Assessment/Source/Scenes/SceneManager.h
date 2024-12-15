#pragma once
#include "Scene.h"
#include <memory>
#include <cwchar>
#include <unordered_map>
class SceneManager
{
public:
	static void AddScene(const wchar_t* name, std::shared_ptr<Scene> sc);

	static std::shared_ptr<Scene> GetActiveScene(); //return the active scene.

private:
	SceneManager();
	~SceneManager();

	static std::unordered_map<const wchar_t*, std::shared_ptr<Scene>> m_scenes; //list of scenes.

	static std::shared_ptr<Scene> m_activeScene; //current active scene. 

};

