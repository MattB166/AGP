#pragma once
#include "Scene.h"
#include <memory>
#include <cwchar>
#include <string>
#include <unordered_map>
class SceneManager
{
public:
	static void AddScene(const std::wstring& name, Scene* sc);

	static Scene* GetActiveScene(); //return the active scene.

	static void SetActiveScene(const wchar_t* name); //set the active scene.

	static void CleanUp(); //clean up the scene manager.

private:
	SceneManager() = delete;
	~SceneManager() = delete;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	static std::unordered_map<std::wstring,std::unique_ptr<Scene>> m_scenes; //list of scenes.

	static std::wstring m_activeScene; //current active scene. 

};

