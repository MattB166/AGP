#include "ShaderSet.h"

ShaderSet::ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il) :
	Component(dev, devcon),
	m_vertexShader(vs),
	m_pixelShader(ps),
	m_inputLayout(il)
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
	//std::cout << "Applying ShaderSet" << std::endl;
	m_devcon->VSSetShader(m_vertexShader, 0, 0);
	m_devcon->PSSetShader(m_pixelShader, 0, 0);
	m_devcon->IASetInputLayout(m_inputLayout);
}
