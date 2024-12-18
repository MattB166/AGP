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
	void ChangeSkyBox(std::shared_ptr<SkyBox> sb); //change the skybox texture.
	void DrawStatics(); //draw all gameobjects in the scene. 
	void CycleSelectedGameObject(); //cycle through the gameobjects in the scene. 
	Camera* GetCamera() { return m_camera; } //get the camera for the scene.

private:
	//vector of gameobjects, and selected gameobject.
	std::vector<GameObject*> m_gameObjects;
	GameObject* m_selectedGameObject = nullptr;
	int m_selectedIndex = 0;
	Camera* m_camera = nullptr; //camera for the scene.
	std::shared_ptr<SkyBox> m_skyBox = nullptr; //skybox for the scene.
};

