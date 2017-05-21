#include "Engine/Renderer/Mesh/Model.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Model::Model()
	: m_meshID(PRIMITIVE_MESH_CUBE)
	, m_material(nullptr)
	, m_materialName("NULL")
{
	m_material	= MaterialBank::GetMaterial("BlinnPhong");
	m_meshRenderer = new MeshRenderer();

	m_model.ChangePosition(Vector3::ZERO);
}


//---------------------------------------------------------------------------------------------------------------------------
Model::Model(const Vector3& position, ePrimitiveMeshType meshType)
	: m_meshID(meshType)
	, m_material(nullptr)
	, m_materialName("NULL")
{
	m_material = MaterialBank::GetMaterial("BlinnPhong");
	m_meshRenderer = new MeshRenderer();

	m_model.ChangePosition(position);
}


//---------------------------------------------------------------------------------------------------------------------------
Model::~Model() {
	delete m_meshRenderer;
}


//---------------------------------------------------------------------------------------------------------------------------
void Model::SetMaterial(const String& matName) {

	m_materialName	= matName;
	m_material		= MaterialBank::GetMaterial(m_materialName);

	if (!m_material) {
		m_material	= MaterialBank::GetMaterial("BlinnPhong");
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Model::ChangeColor(const RGBA& color) {

	m_color				= color;
	Uniform* vertColor	= m_material->GetUniform("gVertColor");
	vertColor->SetData(&m_color);
}





//---------------------------------------------------------------------------------------------------------------------------
float Model::CalcDistFromCamera() const {

	Matrix4 view = *BeirusRenderer::GetViewMatrix();
	Matrix4 proj = *BeirusRenderer::GetProjectionMatrix();
	Matrix4 viewProj = view * proj;

	Vector4 depth = Vector4(m_model.GetPosition(), 1.f) * viewProj;
	return depth.z;

	Vector3 camLoc		= BeirusRenderer::GetPlayerCamera()->m_position;
	Vector3 meshPos		= m_model.GetPosition();
	return DotProduct(camLoc, meshPos - camLoc);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
bool Model::GetCastsShadows() {

	eCastsShadows doesThisSMeshCastShadows = m_renderState.m_castsShadows;

	if (doesThisSMeshCastShadows == CASTS_SHADOWS_DEFAULT) {
		doesThisSMeshCastShadows = m_material->m_renderState.m_castsShadows;
	}

	switch (doesThisSMeshCastShadows) {
	case CASTS_SHADOWS_DEFAULT: {
		ERROR_AND_DIE("Material must define if model casts shadows.");
		break;
	}
	case CASTS_SHADOWS_ON: {
		return true;
		break;
	}
	case CASTS_SHADOWS_OFF: {
		return false;
		break;
	}
	default: {
		ERROR_AND_DIE("Material must define if model casts shadows.");
	}
	}
}