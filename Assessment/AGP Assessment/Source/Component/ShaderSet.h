#pragma once
#include "../Component/Component.h"
class ShaderSet : public Component
{
public:
	ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il,const char* name);
	ShaderSet(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	~ShaderSet();
	void Apply() override;
	void ShowDebugWindow() override;

	ID3D11VertexShader* GetVertexShader() { return m_vertexShader; }
	ID3D11PixelShader* GetPixelShader() { return m_pixelShader; }
	ID3D11InputLayout* GetInputLayout() { return m_inputLayout; }
	std::vector<std::string> GetComponentOptions() const override { return m_AvailableShaderNames; }


private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	static std::vector<std::string> m_AvailableShaderNames; 
};

