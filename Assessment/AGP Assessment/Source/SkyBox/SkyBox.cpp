#include "SkyBox.h"
#include <iostream>
std::vector<std::string> SkyBox::m_AvailableSkyBoxNames{};
std::unordered_map<std::string, std::string> SkyBox::m_SkyBoxNameToPath{};


SkyBox::SkyBox(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjFileModel* model, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il, ID3D11ShaderResourceView* srv, const wchar_t* TexPath)
	: m_device(device), m_deviceContext(deviceContext), m_SkyBoxModel(model), m_vertexShader(vs), m_pixelShader(ps), m_inputLayout(il), m_texture(srv), m_constBuffer(nullptr), m_rasterizerSolid(nullptr), m_rasterizerSkybox(nullptr), m_depthWriteSolid(nullptr), m_depthWriteSkybox(nullptr), m_texturePath(TexPath)

{
	InitialiseSkybox();
}

void SkyBox::Draw(Camera* cam)
{
	if (!m_deviceContext || !m_texture || !m_vertexShader || !m_pixelShader || !m_inputLayout || !m_constBuffer)
	{
		OutputDebugString(L"SkyBox::Draw - One or more required resources are not initialized.");
		std::cout << "SkyBox::Draw - One or more required resources are not initialized." << std::endl;
		return;
	}
	m_deviceContext->OMSetDepthStencilState(m_depthWriteSkybox, 1);
	m_deviceContext->RSSetState(m_rasterizerSkybox);

	//set skybox shaders
	m_deviceContext->PSSetShaderResources(1, 1, &m_texture); ///set the skybox texture to the pixel shader
	m_deviceContext->VSSetShader(m_vertexShader, 0, 0);
	m_deviceContext->PSSetShader(m_pixelShader, 0, 0);
	m_deviceContext->IASetInputLayout(m_inputLayout);

	//set skybox buffers
	CBufferSkyBox cBuffer;
	XMMATRIX translation, proj, view;
	translation = XMMatrixTranslation(cam->GetX(), cam->GetY(), cam->GetZ());
	proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 1000.0f);
	view = cam->GetViewMatrix();
	cBuffer.wvp = translation * view * proj;
	m_deviceContext->UpdateSubresource(m_constBuffer, 0, 0, &cBuffer, 0, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);
	m_deviceContext->PSSetSamplers(0, 1, &m_sampler);
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture);

	m_SkyBoxModel->Draw();
	m_deviceContext->OMSetDepthStencilState(m_depthWriteSolid, 1);
	m_deviceContext->RSSetState(m_rasterizerSolid);

	m_deviceContext->IASetInputLayout(m_inputLayout);

}

void SkyBox::LoadAllSkyBoxNames(const std::string& path)
{
	namespace fs = std::experimental::filesystem;
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (fs::is_regular_file(entry) && entry.path().extension() == ".dds")
		{
			std::string filename = entry.path().filename().string();
			std::string skyboxName = filename.substr(0, filename.find_last_of('.'));
			m_AvailableSkyBoxNames.push_back(skyboxName);
			m_SkyBoxNameToPath.insert(std::make_pair(skyboxName, entry.path().string()));
			//std::cout << "Loaded SkyBox Name : " << skyboxName << "\n";
		}
	}
}

void SkyBox::InitialiseSkybox()
{
	D3D11_RASTERIZER_DESC rsDescSkyBox;
	ZeroMemory(&rsDescSkyBox, sizeof(D3D11_RASTERIZER_DESC));
	rsDescSkyBox.CullMode = D3D11_CULL_BACK;
	rsDescSkyBox.FillMode = D3D11_FILL_SOLID;
	if (FAILED(m_device->CreateRasterizerState(&rsDescSkyBox, &m_rasterizerSolid)))
	{
		OutputDebugString(L"Failed to create rasterizer state");
		return;
	}

	rsDescSkyBox.CullMode = D3D11_CULL_FRONT;
	if (FAILED(m_device->CreateRasterizerState(&rsDescSkyBox, &m_rasterizerSkybox)))
	{
		OutputDebugString(L"Failed to create rasterizer state");
		return;
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	if (FAILED(m_device->CreateDepthStencilState(&dsDesc, &m_depthWriteSolid)))
	{
		OutputDebugString(L"Failed to create depth stencil state");
		return;
	}
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	if (FAILED(m_device->CreateDepthStencilState(&dsDesc, &m_depthWriteSkybox)))
	{
		OutputDebugString(L"Failed to create depth stencil state");
		return;
	}

	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBufferSkyBox);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(m_device->CreateBuffer(&cbd, NULL, &m_constBuffer)))
	{
		OutputDebugString(L"Failed to create constant buffer");
		return;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_device->CreateSamplerState(&samplerDesc, &m_sampler);
}
