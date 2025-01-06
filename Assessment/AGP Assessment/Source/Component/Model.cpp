#include "Model.h"
std::vector<std::string> Model::m_AvailableModelNames;
std::unordered_map<std::string, std::string> Model::m_ModelNameToPath;

Model::Model(ID3D11Device* dev, ID3D11DeviceContext* devcon, ObjFileModel* model, const char* name) : Component(dev, devcon,name,ComponentType::Model), m_model(model)
{
	
}

Model::Model(ID3D11Device* dev, ID3D11DeviceContext* devcon) : Component(dev, devcon, "Model", ComponentType::Model)
{
}

Model::~Model()
{
	//clear maps
	//m_ModelNameToPath.clear();
	//m_AvailableModelNames.clear();
	delete m_model;
	m_model = nullptr;
}

void Model::Apply()
{
	//std::cout << "Applying Model" << std::endl;
	m_model->Draw();
}

void Model::ShowDebugWindow()
{
	//std::cout << "Showing Model Debug Window" << std::endl;
	//ImGui::Text("Model Debug Window");
	ImGui::Text("Model : %s", m_name);
}

void Model::LoadAllModelNames(const std::string& path)
{
	namespace fs = std::experimental::filesystem;
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (fs::is_regular_file(entry) && entry.path().extension() == ".obj")
		{
			std::string filename = entry.path().filename().string();
			std::string modelName = filename.substr(0, filename.find_last_of('.'));
			m_AvailableModelNames.push_back(modelName);
			m_ModelNameToPath.insert(std::make_pair(modelName, entry.path().string()));
			std::cout << "Loaded Model Name : " << modelName << "\n";
		}
	}
}

std::string Model::GetComponentFilePath(const std::string& name) const
{
	auto it = m_ModelNameToPath.find(name);
	if (it != m_ModelNameToPath.end())
	{
		return it->second;
	}
	return "";
}

