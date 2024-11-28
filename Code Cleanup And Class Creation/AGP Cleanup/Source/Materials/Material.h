#pragma once
#include <d3d11.h>
class Material   ////this class will be used to store the shaders and textures for the game objects.
 
{

public:   ////load shaders into material class and then apply them to the device context. 
	  
	Material(const wchar_t* filename, ID3D11Device* g_dev,ID3D11DeviceContext* g_devcon, ID3D11VertexShader* VS, ID3D11PixelShader* PS, ID3D11ShaderResourceView* SRV,ID3D11InputLayout* il);
	~Material();

	void SetTexture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* fileName);
	void SetSampler(ID3D11Device* g_dev);
	void ApplyShaders(ID3D11VertexShader* VS, ID3D11PixelShader* PS,ID3D11InputLayout* il); 
	void Apply(ID3D11DeviceContext* devcon);
	ID3D11ShaderResourceView* GetTexture() { return p_Texture; }
	ID3D11SamplerState* GetSampler() { return p_Sampler; } 
	ID3D11VertexShader* GetVertexShader() { return p_VertexShader; }
	ID3D11PixelShader* GetPixelShader() { return p_PixelShader; }
	ID3D11InputLayout* GetInputLayout() { return m_inputLayout; }
private:

	ID3D11VertexShader* p_VertexShader = NULL;
	ID3D11PixelShader* p_PixelShader = NULL;
	ID3D11ShaderResourceView* p_Texture = NULL;
	ID3D11SamplerState* p_Sampler = NULL; 
	ID3D11InputLayout* m_inputLayout = NULL;
	//ID3D11InputLayout* p_Layout = NULL;

	//needs to be able to return these members to the renderer class so that it can set them in the device context 
};

