#pragma once
#include "Scene.h"
#include <memory>
#include <cwchar>
#include <string>
#include <unordered_map>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class SceneManager
{
public:
	static void Initialise(); //initialise the scene manager.

	static void AddScene(const std::wstring& name);

	static Scene* GetActiveScene(); //return the active scene.

	static void SetActiveScene(const std::wstring& name); //set the active scene.

	static std::string GetActiveSceneName(); //get the active scene name.

	static void CycleActiveScene(); //cycle through the scenes.

	static void InitialiseActiveScene(); //initialise the active scene.

	static void DeleteCurrentScene(); //delete a scene.

	static void AddSkyBoxTextureToActiveScene(const wchar_t* texturePath); //add a skybox to the active scene.

	static void RemoveSkyBoxFromActiveScene(const wchar_t* texturePath); //remove the skybox from the active scene.

	static void SetActiveSkyBoxTexture(const wchar_t* texturePath); //set the active skybox texture.

	static void AddGameObjectToActiveScene(std::unique_ptr<GameObject> obj); //add a gameobject to the active scene.

	static void CycleGameObjectsInActiveScene(); //cycle through the gameobjects in the active scene.

	static GameObject* GetSelectedGameObjectInActiveScene(); //get the selected gameobject in the active scene.

	static const char* GetSelectedGameObjectNameInActiveScene(); //get the selected gameobject name in the active scene.

	static void AddComponentToSelectedGameObjectInActiveScene(std::shared_ptr<Component> comp); //add a component to the selected gameobject in the active scene.

	static void DisplayActiveObjectDebugWindow(); //display the active scene debug window.

	static void CycleSceneSkyBox(); //cycle through the skyboxes in the active scene.

	static void DrawScenePreview(); //draw the active scene preview.

	static void DrawScene(); //draw the active scene. 

	static void MoveActiveSceneCamera(float x, float y, float z); //move the active scene camera.

	static void RotateActiveSceneCamera(float pitch, float yaw); //rotate the active scene camera.

	static void SetActiveSceneCameraTarget(float x, float y, float z, bool TargetMode); //set the active scene camera target.

	static void ResetActiveSceneCamera(); //reset the active scene camera.

	static void ResetActiveObjectPosition(); //reset the active object position.

	static void CleanUp(); //clean up the scene manager.

	static void DisplayDebugWindow();

private:
	SceneManager() = delete;
	~SceneManager() = delete;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	static std::unordered_map<std::wstring,std::unique_ptr<Scene>> m_scenes; //list of scenes.

	static std::unordered_map<std::wstring, std::unique_ptr<Scene>>::iterator m_sceneIterator; //iterator for the scenes.

	static std::wstring m_activeScene; //current active scene name. 

	static std::unordered_map<const wchar_t*, std::shared_ptr<SkyBox>> m_LoadedSkyBoxes; //list of loaded skyboxes. 

	static std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	static std::unique_ptr<DirectX::SpriteFont> m_spriteFont1;

};

