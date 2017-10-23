#include "Engine/Renderer/D3D11/Mesh/D3D11Model.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11MaterialBank.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11Mesh.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11Material.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Model::Model()
	: m_materialName("NULL")
{
	m_material = D3D11MaterialBank::Get()->GetMaterial("BlinnPhong");
	m_meshRenderer = new MeshRenderer();

	m_model.SetPosition(Vector3::ZERO);
}


//---------------------------------------------------------------------------------------------------------------------------
Model::Model(const Vector3& position)
	, m_materialName("NULL")
{
	m_material = D3D11MaterialBank::Get()->GetMaterial("BlinnPhong");
	m_meshRenderer = new D3D11MeshRenderer();

	m_model.SetPosition(position);
}


//---------------------------------------------------------------------------------------------------------------------------
Model::~Model() {
	delete m_meshRenderer;
}


//---------------------------------------------------------------------------------------------------------------------------
void Model::SetMaterial(const String& matName) {

	m_materialName = matName;
	m_material = MaterialBank::GetMaterial(m_materialName);

	if (!m_material) {
		m_material = MaterialBank::GetMaterial("BlinnPhong");
	}
}