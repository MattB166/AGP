#pragma once
#include <d3d11.h>
class Material   ////this class will be used to store the shaders and textures for the game objects. 
 
{

public: 
	Material(ID3D11Device* g_dev, LPCSTR entrypoint, LPCWSTR vertexshaderfile, LPCWSTR pixelshaderfile, ID3D11InputLayout** il);
	~Material();
	HRESULT InitMaterial(ID3D11Device* g_dev, LPCSTR entrypoint, LPCWSTR vertexshaderfile, LPCWSTR pixelshaderfile, ID3D11InputLayout** il);

	void SetTexture(ID3D11ShaderResourceView* texture, ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* fileName);
	void SetSampler(ID3D11SamplerState* sampler, ID3D11Device* g_dev);
	ID3D11ShaderResourceView* GetTexture() { return p_Texture; }
	ID3D11SamplerState* GetSampler() { return p_Sampler; }
	ID3D11VertexShader* GetVertexShader() { return p_VS; }
	ID3D11PixelShader* GetPixelShader() { return p_PS; } 
	ID3D11InputLayout* GetInputLayout() { return p_Layout; }
	ID3D11Buffer* GetCBuffer() { return p_CBuffer; }
	//set c buffer up once i know what it needs to be 

private:
	//void LoadVertexShader(LPCWSTR filename, LPCSTR entrypoint);
	//void LoadPixelShader(LPCWSTR filename, LPCSTR entrypoint);
	ID3D11VertexShader* p_VS = NULL;
	ID3D11PixelShader* p_PS = NULL;
	ID3D11InputLayout* p_Layout = NULL;
	ID3D11Buffer* p_CBuffer = NULL;  ///how do i use this?? 

	ID3D11ShaderResourceView* p_Texture = NULL;
	ID3D11SamplerState* p_Sampler = NULL;

	//needs to be able to return these members to the renderer class so that it can set them in the device context 
};

