#include "Model.h"
std::vector<std::string> Model::m_AvailableModelNames = {};

Model::Model(ID3D11Device* dev, ID3D11DeviceContext* devcon, ObjFileModel* model, const char* name) : Component(dev, devcon,name,ComponentType::Model), m_model(model)
{

}

Model::~Model()
{
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
			m_AvailableModelNames.push_back(entry.path().filename().string());
			std::string modelName = filename.substr(0, filename.find_last_of('.'));
			std::cout << "Loaded Model : " << modelName << "\n";
		}
	}
}

