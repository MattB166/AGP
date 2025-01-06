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
	bool isVertexShader = false;
	bool isPixelShader = false;
	////////////////////////////////////////NEEDS WORK ////////////////////////////////////////////
	for (const auto& entry : fs::directory_iterator(path))
	{
		ShaderPath shaderPath;
		if (fs::is_regular_file(entry))
		{
			std::string extension = entry.path().extension().string();
			std::string filename = entry.path().filename().string();
			std::string folderName = entry.path().parent_path().filename().string();
			std::string shaderName = filename.substr(0, filename.find_last_of('.'));
			if (extension == ".hlsl" || extension == ".cso")
			{
				
				if (filename.find("Vertex") != std::string::npos || filename.find("VShader") != std::string::npos)
				{
					shaderPath.vertexShaderPath = entry.path().string();
				}
				else if (filename.find("Pixel") != std::string::npos || filename.find("PShader") != std::string::npos)
				{
					shaderPath.pixelShaderPath = entry.path().string();
				}

				auto it = m_shaderNameToPaths.find(folderName);
				if (it != m_shaderNameToPaths.end())
				{
					std::cout << "folder already exists" << std::endl;
					if (!shaderPath.vertexShaderPath.empty())
					{
						it->second.vertexShaderPath = shaderPath.vertexShaderPath;
						std::cout << "Assigned Vertex Shader Path : " << shaderPath.vertexShaderPath << std::endl;
					}
					if (!shaderPath.pixelShaderPath.empty())
					{
						it->second.pixelShaderPath = shaderPath.pixelShaderPath;
						std::cout << "Assigned Pixel Shader Path : " << shaderPath.pixelShaderPath << std::endl;
					}
				}
				else
				{
					m_shaderNameToPaths[folderName] = shaderPath;
				}

				// Add the shader name to the available shader names list
				if (std::find(m_AvailableShaderNames.begin(), m_AvailableShaderNames.end(), folderName) == m_AvailableShaderNames.end())
				{
					m_AvailableShaderNames.push_back(folderName);
					std::cout << "Loaded Shader Name : " << folderName << "\n";
				}
			}
			
		}
	}
}

std::string ShaderSet::GetComponentFilePath(const std::string& name) const
{
	auto it = m_shaderNameToPaths.find(name);
	if (it != m_shaderNameToPaths.end())
	{
		return it->second.vertexShaderPath + it->second.pixelShaderPath;
	}
	return "";

}
