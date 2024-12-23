#pragma once
#include "../Component/Component.h"
#include <d3d11.h>
#include <memory>
class Material : public Component ///deals with the texture of the object. 
{
public:
	Material(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11ShaderResourceView* texture, const char* name);
	~Material();
	void Apply() override;
	void ShowDebugWindow() override;

	void SetTexture(ID3D11ShaderResourceView* texture) { m_texture = texture; }
	ID3D11ShaderResourceView* GetTexture() { return m_texture; }

private:
	ID3D11ShaderResourceView* m_texture = nullptr;
	ID3D11SamplerState* m_sampler = nullptr;
	
};

