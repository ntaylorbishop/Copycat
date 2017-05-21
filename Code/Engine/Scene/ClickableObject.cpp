#include "Engine/Scene/ClickableObject.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Mesh/Model.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"


STATIC std::vector<ClickableObject*> ClickableObject::s_sceneClickableObjs;
STATIC uint32_t ClickableObject::s_id = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
ClickableObject::ClickableObject(eGeomType type, const Vector3& pos)
	: m_id(s_id++)
	, m_pos(pos)
{ 
	s_sceneClickableObjs.push_back(this);

	Matrix4 view = *BeirusRenderer::GetViewMatrix();
	view.TransposeRotation();

	m_viewDir.x = view.GetThirdRow().x;
	m_viewDir.y = view.GetThirdRow().y;
	m_viewDir.z = view.GetThirdRow().z;

	m_outlineMat = MaterialBank::GetMaterial("Outline");

	m_outlineMat->CreateUniform("gViewDir", UNIFORM_VECTOR3, 1, &m_viewDir);
	m_outlineMat->m_renderState.m_backfaceCulling = true;

	m_model.ChangePosition(pos);

	m_geom = CreateGeomType(type);
}


//---------------------------------------------------------------------------------------------------------------------------
ClickableObject::~ClickableObject() {

	uint32_t i = 0;
	for (i; i < s_sceneClickableObjs.size(); i++) {
		
		if (s_sceneClickableObjs[i] == this) {
			break;
		}
	}

	ASSERT_OR_DIE(i != s_sceneClickableObjs.size(), "ERROR: ClickableObject not a part of list.");
	s_sceneClickableObjs.erase(s_sceneClickableObjs.begin() + i);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void ClickableObject::Update(float deltaSeconds) {

}


//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void ClickableObject::Render() const {

	if (m_isSelected) {
		BeirusRenderer::DisableDepthTesting();
		MeshRenderer* mr = BeirusRenderer::GetGlobalMeshRenderer();
		mr->RenderMeshWithMaterial(PRIMITIVE_MESH_SPHERE_100, m_outlineMat, m_model);
		BeirusRenderer::EnableDepthTesting();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SORT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
float ClickableObject::CalcDistFromCamera() const {

	Matrix4 view = *BeirusRenderer::GetViewMatrix();
	Matrix4 proj = *BeirusRenderer::GetProjectionMatrix();
	Matrix4 viewProj = view * proj;

	Vector4 depth = Vector4(m_pos, 1.f) * viewProj;
	return depth.z;
}


//---------------------------------------------------------------------------------------------------------------------------
bool ClickableObjectComparator(ClickableObject* i, ClickableObject* j) {

	return (i->CalcDistFromCamera() > j->CalcDistFromCamera());
}
//---------------------------------------------------------------------------------------------------------------------------
STATIC void ClickableObject::SortByDistanceToCamera() {

	std::sort(s_sceneClickableObjs.begin(), s_sceneClickableObjs.end(), ClickableObjectComparator);
}