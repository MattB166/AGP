#include "GameObject.h"
#include "../AssetManager/AssetManager.h"

GameObject::GameObject(const char* name, bool reflective) : m_name(name), m_reflectiveObject(reflective)
{
	Initialise();
	AssetManager::CreateConstantBuffer(); 
}

GameObject::~GameObject()
{
	//clear components
	m_components.clear();
	std::cout << "Gameobject Deleted" << std::endl;

}

void GameObject::Initialise()
{
	//position, rotation, scale
	m_transform.pos = { 0,0,1 };
	m_transform.rot = { 0,0,0 }; 
	m_transform.scl = { 1,1,1 };

	shaderSet = AssetManager::CreateShaderSet(L"CompiledShaders/StandardShader/VertexShader.cso", L"CompiledShaders/StandardShader/PixelShader.cso", "Basic Shader");
	reflectiveShaderSet = AssetManager::CreateShaderSet(L"CompiledShaders/ReflectiveShader/ReflectiveVertexShader.cso", L"CompiledShaders/ReflectiveShader/ReflectivePixelShader.cso", "Reflective Shader");
	if (m_reflectiveObject)
		AddComponent(reflectiveShaderSet);
	else
		AddComponent(shaderSet);
	std::shared_ptr<Material> mat = AssetManager::CreateMaterial(L"Source/SavedTextures/Box.bmp", "Box Texture"); //adding placeholder components which can be switched out.
	AddComponent(mat);
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	if (component == nullptr)
	{
		std::cout << "Component is null" << std::endl;
		return;

	}
	//check to make sure doesnt already exist
	for (const auto& comp : m_components)
	{
		if (comp == component)
		{
			return;
		}
		if (comp->GetType() == component->GetType())
		{
			std::cout << "Component already exists" << std::endl;
			RemoveComponent(comp);
		}
		
	}
	component->SetOwner(this);
	m_components.push_back(component);
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	auto it = std::find(m_components.begin(), m_components.end(), component);
	if (it != m_components.end())
	{
		m_components.erase(it);
	}
}

void GameObject::Draw(XMMATRIX& view, XMMATRIX& proj)
{

	XMMATRIX world = m_transform.GetWorldMatrix();

	m_constantBuffer.WVP = world * view * proj;
	m_constantBuffer.WV = world * view;
	m_constantBuffer.ambientLightCol = { BasicAmbientLightValue,BasicAmbientLightValue,BasicAmbientLightValue,1.0f };
	XMVECTOR directionalLightShinesFrom = { 0.2788f,0.7063f,0.6506f }; //make this a member so can adjust it in runtime. 
	XMMATRIX transpose = XMMatrixTranspose(m_transform.GetWorldMatrix());
	m_constantBuffer.directionalLightDir = XMVector3Transform(directionalLightShinesFrom, transpose);


	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if (!m_constantBuffer.pointLights[i].enabled)
			continue;

		XMMATRIX inverse = XMMatrixInverse(nullptr, m_transform.GetWorldMatrix());
		if (!m_constantBuffer.pointLights[i].enabled)
			m_constantBuffer.pointLights[i].position = XMVector3Transform(m_constantBuffer.pointLights[i].position, inverse);
		m_constantBuffer.pointLights[i].colour = m_constantBuffer.pointLights[i].colour;
		m_constantBuffer.pointLights[i].strength = m_constantBuffer.pointLights[i].strength;
		m_constantBuffer.pointLights[i].enabled = m_constantBuffer.pointLights[i].enabled;

	}
	AssetManager::UpdateConstantBuffer(m_constantBuffer);



	for (auto component : m_components)
	{
		component->Apply();
	}
	//std::cout << "Position: " << m_transform.pos.x << " " << m_transform.pos.y << " " << m_transform.pos.z << std::endl;

}

void GameObject::ShowComponentDebugWindow()
{
	ImGui::Text("Current Object: %s", std::string(m_name.begin(), m_name.end()).c_str());
	//logic to add the components to the debug window
	if (ImGui::Button("Add Component"))
	{
		//selectedComponentType = ComponentType::None;
		ImGui::OpenPopup("Add Component");
	}
	if (ImGui::BeginPopupModal("Add Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		std::vector<ComponentType> types = Component::GetTypes();
		types.erase(std::remove(types.begin(), types.end(), ComponentType::Shaders), types.end());
		types.erase(std::remove(types.begin(), types.end(), ComponentType::Texture), types.end());
		for (auto type : types)
		{
			std::string typeName = Component::ComponentTypeToString(type);
			if (ImGui::Button(typeName.c_str()))
			{
				selectedComponentType = type;
				std::cout << "Selected Component Type: " << typeName << std::endl;
				showComponentOptions = true;

				PreComponentAddition(type);
				ImGui::CloseCurrentPopup();
			}


		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			showComponentOptions = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (showComponentOptions)
	{
		ImGui::OpenPopup("Component Options");
	}
	if (ImGui::BeginPopupModal("Component Options"))
	{
		//show options for the selected component type.
		for (const auto& option : options)
		{
			if (ImGui::Button(option.c_str()))
			{
				/////need different way to account for shaders. 
				std::string fPath = temp->GetComponentFilePath(option);
				std::shared_ptr<Component> comp = AssetManager::CreateComponentFromFilePath(fPath, selectedComponentType, option.c_str()); ////CURRENTLY FAILS HERE WITH SHADERS 
				AddComponent(comp);
				showComponentOptions = false;
				ImGui::CloseCurrentPopup();

			}
		}

		ImGui::Separator();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			showComponentOptions = false;
			//options.clear();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	if (ImGui::Button("Edit Component"))
	{
		//selectedComponentType = ComponentType::None;
		ImGui::OpenPopup("Components");
	}
	if (ImGui::BeginPopupModal("Components", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		std::vector<ComponentType> types = Component::GetTypes();
		types.erase(std::remove(types.begin(), types.end(), ComponentType::Shaders), types.end()); //dont want to manually be messing with shaders. will let behind the scenes handle it. 
		for (auto type : types)
		{
			if (ImGui::Button(Component::ComponentTypeToString(type).c_str()))
			{
				//give options to switch component type 
				ImGui::CloseCurrentPopup();
			}
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::Text("Position : %f %f %f", m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);
	ImGui::SliderFloat("Position Snapping value", &DebugMovementSnappingValue, 0.05f, 1.0f);
	ImGui::DragFloat("Position X", &m_transform.pos.x, DebugMovementSnappingValue, -30.0f, 30.0f,"%.3f");
	ImGui::DragFloat("Position Y", &m_transform.pos.y, DebugMovementSnappingValue, - 30.0f, 30.0f, "%.3f");
	ImGui::DragFloat("Position Z", &m_transform.pos.z, DebugMovementSnappingValue, - 30.0f, 30.0f, "%.3f");
	if (ImGui::Button("Reset Position"))
	{
		m_transform.pos = { 0,0,1 };
	}


	ImGui::Text("Rotation : %f %f %f", m_transform.rot.x, m_transform.rot.y, m_transform.rot.z);
	ImGui::DragFloat("Rotation X", &m_transform.rot.x, 0.01f, -XM_2PI, XM_2PI, "%.3f");
	ImGui::DragFloat("Rotation Y", &m_transform.rot.y, 0.01f, -XM_2PI, XM_2PI, "%.3f");
	ImGui::DragFloat("Rotation Z", &m_transform.rot.z, 0.01f, -XM_2PI, XM_2PI, "%.3f");
	if (ImGui::Button("Reset Rotation"))
	{
		m_transform.rot = { 0,0,0 };
	}


	ImGui::Text("Scale : %f %f %f", m_transform.scl.x, m_transform.scl.y, m_transform.scl.z);
	ImGui::DragFloat("Scale X", &m_transform.scl.x, 0.01f, 0.1f, 10.0f, "%.3f");
	ImGui::DragFloat("Scale Y", &m_transform.scl.y, 0.01f, 0.1f, 10.0f, "%.3f");
	ImGui::DragFloat("Scale Z", &m_transform.scl.z, 0.01f, 0.1f, 10.0f, "%.3f");
	if (ImGui::Button("Reset Scale"))
	{
		m_transform.scl = { 1,1,1 };
	}
	
	ImGui::SliderFloat("Ambient Light", &BasicAmbientLightValue, 0.0f, 1.0f);

	if (m_components.size() > 0)
	{
		for (auto component : m_components)
		{
			component->ShowDebugWindow();
		}
	}
	/*if (ImGui::Checkbox("Reflective Object", &m_reflectiveObject))
	{
		ToggleReflectiveObject();
	}
	*/

}

void GameObject::ToggleReflectiveObject()
{
	
}

void GameObject::PreComponentAddition(ComponentType type)
{
	options.clear();
	temp = AssetManager::CreateTemporaryComponentInstance(type);
	if (temp)
	{
		options = temp->GetComponentOptions();
		std::cout << "Options size" << options.size() << std::endl;
		for (const auto& option : options)
		{
			std::cout << option << std::endl;
		}
	}
	else
	{
		std::cout << "Component is null" << std::endl;
	}
}


