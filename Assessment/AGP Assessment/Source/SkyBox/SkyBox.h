#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <d3d11.h>
#include "../Constant Buffer/CBuffer.h" //maybe make this class a component but don't allow gameobjects to have one of these ones. 
#include "../ObjectFileModel/objfilemodel.h"
#include <unordered_map>
#include "../Camera/Camera.h"
#include <experimental/filesystem>
using namespace std::experimental::filesystem;
struct CBufferSkyBox
{
	XMMATRIX wvp;
};
class SkyBox
{
public: 

	SkyBox(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjFileModel* model, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il, ID3D11ShaderResourceView* srv, const wchar_t* TexPath);
	
	void Draw(Camera* cam); 

	const wchar_t* GetTexturePath() { return m_texturePath; } 

	static void LoadAllSkyBoxNames(const std::string& path);

	std::vector<std::string> GetSkyBoxOptions() const { return m_AvailableSkyBoxNames; }

private:
	void InitialiseSkybox();


	ID3D11ShaderResourceView* m_texture = nullptr;

	ID3D11RasterizerState* m_rasterizerSkybox = nullptr;

	ID3D11RasterizerState* m_rasterizerSolid = nullptr;

	ID3D11DepthStencilState* m_depthWriteSolid = nullptr;

	ID3D11DepthStencilState* m_depthWriteSkybox = nullptr;

	ID3D11VertexShader* m_vertexShader = nullptr;

	ID3D11PixelShader* m_pixelShader = nullptr;

	ID3D11InputLayout* m_inputLayout = nullptr;

	ID3D11Device* m_device = nullptr;

	ID3D11DeviceContext* m_deviceContext = nullptr;

	ObjFileModel* m_SkyBoxModel = nullptr; 

	ID3D11Buffer* m_constBuffer = nullptr;

	ID3D11SamplerState* m_sampler = nullptr;

	const wchar_t* m_texturePath; 

	static std::vector<std::string> m_AvailableSkyBoxNames;

	static std::unordered_map<std::string, std::string> m_SkyBoxNameToPath;

};

