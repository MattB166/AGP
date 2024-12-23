#include "Model.h"

Model::Model(ID3D11Device* dev, ID3D11DeviceContext* devcon, ObjFileModel* model) : Component(dev, devcon), m_model(model)
{

}

Model::~Model()
{
	delete m_model;
	m_model = nullptr;
}

void Model::Apply()
{
	//std::cout << "Applying Model" << std::endl;
	m_model->Draw();
}

