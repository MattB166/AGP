#pragma once
#include "../ObjectFileModel/objfilemodel.h"
#include "../Component/Component.h"
#include <d3d11.h>
class Model : public Component
{
public:
	Model(ID3D11Device* dev, ID3D11DeviceContext* devcon, ObjFileModel* model, const char* name);
	Model(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	~Model();
	void Apply() override;
	void ShowDebugWindow() override;
	void SetModel(ObjFileModel* model) { m_model = model; }
	static void LoadAllModelNames(const std::string& path);
	ObjFileModel* GetModel() { return m_model; }
	std::vector<std::string> GetComponentOptions() const override { return m_AvailableModelNames; }
private:
	ObjFileModel* m_model = nullptr;
	ID3D11Buffer* m_constBuffer = nullptr;
	static vector<std::string> m_AvailableModelNames; 
	///need a vector of all models available through the file explorer 
};

