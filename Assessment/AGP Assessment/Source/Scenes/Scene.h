#pragma once
#include "../GameObject/GameObject.h"
#include "../Camera/Camera.h"
#include "../SkyBox/SkyBox.h"
class Scene //controls all current gameobjects in the scene. 
{
public:
	Scene(); //constructor for the scene.
	~Scene(); //destructor for the scene.
	void Initialize(); //initialize the scene.
	void ChangeActiveSkyBox(std::shared_ptr<SkyBox> sb); //change the skybox texture.
	void AddSkyBoxToScene(std::shared_ptr<SkyBox> sb); //add a skybox to the scene.
	void RemoveSkyBoxFromScene(std::shared_ptr<SkyBox> sb); //remove a skybox from the scene.
	void CycleThroughSkyBoxes(); //cycle through the skyboxes in the scene.
	void DrawStatics(); //draw all gameobjects in the scene. 
	void CycleSelectedGameObject(); //cycle through the gameobjects in the scene. 
	Camera* GetCamera() { return m_camera; } //get the camera for the scene.

private:
	
	std::vector<GameObject*> m_gameObjects;
	GameObject* m_selectedGameObject = nullptr;
	int m_selectedObjectIndex = 0;

	Camera* m_camera = nullptr; 

	
	std::vector<std::shared_ptr<SkyBox>> m_skyBoxes; 
	std::shared_ptr<SkyBox> m_ActiveSkyBox = nullptr; 
	int m_skyBoxIndex = 0; 
};

