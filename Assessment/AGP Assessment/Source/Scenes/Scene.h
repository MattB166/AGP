#pragma once
#include "../GameObject/GameObject.h"
#include "../Camera/Camera.h"
#include "../SkyBox/SkyBox.h"
#include <memory>
#include <queue>
#include <vector>

class Scene //controls all current gameobjects in the scene. 
{
public:
	Scene(const std::wstring name); //constructor for the scene.
	~Scene(); //destructor for the scene.
	void Initialize(); //initialize the scene.
	void Update(); //update the scene and process deletions. 
	void ShowDebugWindow(); //show the debug window for the scene.
	void ChangeActiveSkyBox(std::shared_ptr<SkyBox> sb); //change the skybox texture.
	void AddSkyBoxToScene(std::shared_ptr<SkyBox> sb); //add a skybox to the scene.
	void RemoveSkyBoxFromScene(std::shared_ptr<SkyBox> sb); //remove a skybox from the scene.
	void AddGameObject(std::unique_ptr<GameObject> obj); //add a gameobject to the scene.
	void RemoveActiveGameObject(); //remove a gameobject from the scene.
	void CycleThroughSkyBoxes(); //cycle through the skyboxes in the scene.
	void DrawStatics(); //draw all gameobjects in the scene. 
	void CycleSelectedGameObject(); //cycle through the gameobjects in the scene. 
	GameObject* GetSelectedGameObject() { return m_selectedGameObject; } //get the selected gameobject in the scene.
	int GetObjectCount() { return m_gameObjects.size(); } //get the number of gameobjects in the scene.
	Camera* GetCamera() { return m_ActiveCamera; } //get the camera for the scene.
	void MoveActiveCamera(float x, float y, float z); //move the camera in the scene.
	void RotateActiveCamera(float pitch, float yaw); //rotate the camera in the scene.
	void AddCamera(); //add a camera to the scene.

	std::string GetName() { return std::string(m_name.begin(), m_name.end()); } //get the name of the scene.
	std::vector<std::shared_ptr<SkyBox>> GetSkyBoxes() { return m_skyBoxes; } //get the skyboxes in the scene.

	void DisplaySceneDebugWindow(); //display the scene debug window.


private:
	
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	GameObject* m_selectedGameObject = nullptr;
	int m_selectedObjectIndex = 0;

	Camera* m_ActiveCamera = nullptr; 
	std::vector<Camera*> m_cameras;
	int m_cameraIndex = 0;

	
	std::vector<std::shared_ptr<SkyBox>> m_skyBoxes; 
	std::shared_ptr<SkyBox> m_ActiveSkyBox = nullptr; 
	int m_skyBoxIndex = 0; 

	std::wstring m_name; //name of the scene.
	//think about a set position for previewing objects, and moving the camera around the scene. 

	//deletion queue for gameobjects.
	//std::vector<std::unique_ptr<GameObject>> m_deletionQueue;
};

