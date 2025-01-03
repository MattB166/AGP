#include "ShaderSet.h"
std::vector<std::string> ShaderSet::m_AvailableShaderNames;

ShaderSet::ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il, const char* name) :
	Component(dev, devcon,name,ComponentType::Shaders),
	m_vertexShader(vs),
	m_pixelShader(ps),
	m_inputLayout(il)
{
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
