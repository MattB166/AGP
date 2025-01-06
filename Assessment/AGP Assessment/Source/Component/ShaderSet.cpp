#include "ShaderSet.h"
std::vector<std::string> ShaderSet::m_AvailableShaderNames;
std::unordered_map<std::string, ShaderPath> ShaderSet::m_shaderNameToPaths;

ShaderSet::ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il, const char* name, std::string& vsPath, std::string& psPath) :
	Component(dev, devcon,name,ComponentType::Shaders),
	m_vertexShader(vs),
	m_pixelShader(ps),
	m_inputLayout(il),
	m_vertexShaderPath(vsPath),
	m_pixelShaderPath(psPath)
{
	//std::cout << "Path: " << vsPath << " " << psPath << std::endl;
}

ShaderSet::ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon) :
	Component(dev, devcon, "Shader Set", ComponentType::Shaders), m_vertexShader(nullptr), m_pixelShader(nullptr), m_inputLayout(nullptr)
{
}

ShaderSet::~ShaderSet()
{
	if (m_vertexShader) m_vertexShader->Release();
	if (m_pixelShader) m_pixelShader->Release();
	if (m_inputLayout) m_inputLayout->Release();
}


void ShaderSet::Apply()
{
	m_devcon->VSSetShader(m_vertexShader, 0, 0);
	m_devcon->PSSetShader(m_pixelShader, 0, 0);
	m_devcon->IASetInputLayout(m_inputLayout);
}

void ShaderSet::ShowDebugWindow()
{
	//ImGui::Text("ShaderSet");
	ImGui::Text("Shader : %s" , m_name);
}

void ShaderSet::LoadAllShaderNames(const std::string& path)
{
	namespace fs = std::experimental::filesystem;
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (fs::is_regular_file(entry))
		{
			std::string extension = entry.path().extension().string();
			std::string filename = entry.path().filename().string();
			std::string folderName = entry.path().parent_path().filename().string();
			std::string shaderName = filename.substr(0, filename.find_last_of('.'));
			std::cout << "Folder Name of this shader: " + folderName << std::endl;
		}
	}
}

std::string ShaderSet::GetComponentFilePath(const std::string& name) const
{
	auto it = m_shaderNameToPaths.find(name);
	if (it != m_shaderNameToPaths.end())
	{
		return "Vertex Shader: " + it->second.vertexShaderPath + " Pixel Shader: " + it->second.pixelShaderPath;
	}
	return "";

}
