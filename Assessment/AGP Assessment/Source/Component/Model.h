#pragma once
#include "../ObjectFileModel/objfilemodel.h"
#include "../Component/Component.h"
#include <d3d11.h>
class Model : public Component
{
public:
	Model(ID3D11Device* dev, ID3D11DeviceContext* devcon, ObjFileModel* model, const char* name);
	~Model();
	void Apply() override;
	void ShowDebugWindow() override;
	void SetModel(ObjFileModel* model) { m_model = model; }
	ObjFileModel* GetModel() { return m_model; }
private:
	ObjFileModel* m_model = nullptr;
	ID3D11Buffer* m_constBuffer = nullptr;
};

