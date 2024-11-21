#pragma once
#include <d3d11.h>
using namespace DirectX;
class Material
{

public: 
	Material();
	~Material();
	HRESULT InitMaterial(ID3D11Device* g_dev, LPCSTR entrypoint, LPCWSTR vertexshaderfile, LPCWSTR pixelshaderfile, ID3D11InputLayout** il);

	void SetTexture(ID3D11ShaderResourceView* texture, ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* fileName);
	void SetSampler(ID3D11SamplerState* sampler, ID3D11Device* g_dev);
	ID3D11ShaderResourceView* GetTexture() { return p_Texture; }
	ID3D11SamplerState* GetSampler() { return p_Sampler; }

private:
	//void LoadVertexShader(LPCWSTR filename, LPCSTR entrypoint);
	//void LoadPixelShader(LPCWSTR filename, LPCSTR entrypoint);
	ID3D11VertexShader* p_VS = NULL;
	ID3D11PixelShader* p_PS = NULL;
	ID3D11InputLayout* p_Layout = NULL;
	ID3D11Buffer* p_CBuffer = NULL;

	ID3D11ShaderResourceView* p_Texture = NULL;
	ID3D11SamplerState* p_Sampler = NULL;

	//needs to be able to return these members to the renderer class so that it can set them in the device context 
};

