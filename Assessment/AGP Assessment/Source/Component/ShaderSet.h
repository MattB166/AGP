#pragma once
#include "../Component/Component.h"
struct ShaderPath
{
	std::string pixelShaderPath;
	std::string vertexShaderPath;

	std::string GetPixelShaderPath() { return pixelShaderPath; }
	std::string GetVertexShaderPath() { return vertexShaderPath; }
};
class ShaderSet : public Component
{
public:
	ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il,const char* name,std::string& vsPath, std::string& psPath);
	ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	~ShaderSet();
	void Apply() override;
	void ShowDebugWindow() override;

	static void LoadAllShaderNames(const std::string& path);
	ID3D11VertexShader* GetVertexShader() { return m_vertexShader; }
	ID3D11PixelShader* GetPixelShader() { return m_pixelShader; }
	ID3D11InputLayout* GetInputLayout() { return m_inputLayout; }
	std::vector<std::string> GetComponentOptions() const override { return m_AvailableShaderNames; }
	std::string GetComponentFilePath(const std::string& name)const override;


private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	std::string m_pixelShaderPath;
	std::string m_vertexShaderPath;
	static std::vector<std::string> m_AvailableShaderNames; 
	std::unordered_map<std::string, std::string> m_shaderPaths;
	static std::unordered_map<std::string, ShaderPath> m_shaderNameToPaths;

};

