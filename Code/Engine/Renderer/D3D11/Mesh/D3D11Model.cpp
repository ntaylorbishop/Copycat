#include "Engine/Renderer/D3D11/Mesh/D3D11Model.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11MaterialBank.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11Mesh.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11Material.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
D3D11Model::D3D11Model() {

	m_material = D3D11MaterialBank::Get()->GetMaterial("BlinnPhong");
	m_meshRenderer = new D3D11MeshRenderer();

	m_model.SetPosition(Vector3::ZERO);
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Model::D3D11Model(const Vector3& position) {

	m_material = D3D11MaterialBank::Get()->GetMaterial("BlinnPhong");
	m_meshRenderer = new D3D11MeshRenderer();

	m_model.SetPosition(position);
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Model::~D3D11Model() {
	delete m_meshRenderer;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Model::SetMaterial(const String& matName) {

	m_materialName = matName;
	m_material = D3D11MaterialBank::Get()->GetMaterial(m_materialName);

	if (!m_material) {
		m_material = D3D11MaterialBank::Get()->GetMaterial("BlinnPhong");
	}
}