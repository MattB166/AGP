#include "GameObject.h"
#include "../AssetManager/AssetManager.h"

GameObject::GameObject(const char* name) : m_name(name)
{
	Initialise();
	AssetManager::CreateConstantBuffer(); 
}

GameObject::~GameObject()
{
	//clear components
	m_components.clear();
	if (m_name)
	{
		delete m_name;
		m_name = nullptr;
	}


}

void GameObject::Initialise()
{
	//position, rotation, scale
	m_transform.pos = { 0,0,3 };
	m_transform.rot = { 0,0,0 }; 
	m_transform.scl = { 1,1,1 };
	std::shared_ptr<ShaderSet> shaderSet = AssetManager::CreateShaderSet(L"CompiledShaders/VertexShader.cso", L"CompiledShaders/PixelShader.cso", "Standard Shader");
	AddComponent(shaderSet);
	std::shared_ptr<Material> mat = AssetManager::CreateMaterial(L"Source/SavedTextures/Box.bmp", "Box Texture");
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
	ImGui::Text( " % s", m_name);
	//logic to add the components to the debug window

	ImGui::Text("Position : %f %f %f", m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);
	ImGui::SliderFloat("Position Snapping value", &DebugMovementSnappingValue, 0.05f, 1.0f);
	ImGui::DragFloat("Position X", &m_transform.pos.x, DebugMovementSnappingValue, -30.0f, 30.0f,"%.3f");
	ImGui::DragFloat("Position Y", &m_transform.pos.y, DebugMovementSnappingValue, - 30.0f, 30.0f, "%.3f");
	ImGui::DragFloat("Position Z", &m_transform.pos.z, DebugMovementSnappingValue, - 30.0f, 30.0f, "%.3f");

	
	ImGui::SliderFloat("Ambient Light", &BasicAmbientLightValue, 0.0f, 1.0f);

	if (m_components.size() > 0)
	{
		for (auto component : m_components)
		{
			component->ShowDebugWindow();
		}
	}
	ImGui::Checkbox("Reflective Object", &m_reflectiveObject);
	

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
}

void GameObject::ToggleReflectiveObject()
{
	m_reflectiveObject = !m_reflectiveObject;

	if (m_reflectiveObject)
	{
		//AssetManager::CreateShaderSet
	}
	else
	{
		std::shared_ptr<ShaderSet> shaderSet = AssetManager::CreateShaderSet(L"CompiledShaders/VertexShader.cso", L"CompiledShaders/PixelShader.cso", "Standard Shader");
		AddComponent(shaderSet);
	}
}


