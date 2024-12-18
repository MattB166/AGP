#include "SkyBox.h"

SkyBox::SkyBox(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjFileModel* model, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il, ID3D11ShaderResourceView* srv)
	: m_device(device), m_deviceContext(deviceContext), m_SkyBoxModel(model), m_vertexShader(vs), m_pixelShader(ps), m_inputLayout(il), m_texture(srv)
{
}

void SkyBox::Draw(Camera* cam)
{
	m_deviceContext->OMSetDepthStencilState(m_depthWriteSkybox, 1);
	m_deviceContext->RSSetState(m_rasterizerSkybox);

	//set skybox shaders
	m_deviceContext->PSSetShaderResources(1, 1, &m_texture); ///set the skybox texture to the pixel shader
	m_deviceContext->VSSetShader(m_vertexShader, 0, 0);
	m_deviceContext->PSSetShader(m_pixelShader, 0, 0);
	m_deviceContext->IASetInputLayout(m_inputLayout);

	//set skybox buffers
	//CBufferSkyBox cBuffer;
	//XMMATRIX translation, proj, view;
	//translation = XMMatrixTranslation(cam->GetX(), cam->GetY(), cam->GetZ());
	//proj = cam->GetProjectionMatrix();
	//view = cam->GetViewMatrix();
	//cBuffer.wvp = translation * view * proj;
	//m_deviceContext->UpdateSubresource(m_constBuffer, 0, 0, &cBuffer, 0, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constBuffer);
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture);

	m_SkyBoxModel->Draw();
	m_deviceContext->OMSetDepthStencilState(m_depthWriteSolid, 1);
	m_deviceContext->RSSetState(m_rasterizerSolid);

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
}
