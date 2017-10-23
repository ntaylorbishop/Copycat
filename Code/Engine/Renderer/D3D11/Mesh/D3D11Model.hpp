#pragma once

#define _USE_MATH_DEFINES

#include "Engine/Renderer/General/RenderCommon.hpp"
#include "Engine/Math/Matrix44.hpp"


class D3D11MeshRenderer;
class D3D11Mesh;
class D3D11Material;

class D3D11Model {
public:
	//STRUCTORS
	D3D11Model();
	D3D11Model(const Vector3& position);
	~D3D11Model();

	void SetMaterial(const String& matName);

	void Render(MeshRenderer* customMeshRenderer = nullptr) const;
	void Render(Material* matToRenderWith) const;

	const Matrix4* GetModel() { return &m_model; }

public:
	D3D11MeshRenderer*	m_meshRenderer	= nullptr;
	D3D11Mesh*			m_mesh			= nullptr;
	D3D11Material*		m_material		= nullptr;
	String				m_materialName	= "NULL";
	Matrix44			m_model			= Matrix44::IDENTITY;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Model::Render(MeshRenderer* customMeshRenderer) const {

	customMeshRenderer->RenderMeshWithMaterial(m_meshID, m_material, m_model);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Model::Render(Material* matToRenderWith) const {

	m_meshRenderer->RenderMeshWithMaterial(m_meshID, matToRenderWith, m_model);
}