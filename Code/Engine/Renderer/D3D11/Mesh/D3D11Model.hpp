#pragma once

#define _USE_MATH_DEFINES

#include "Engine/Renderer/General/RenderCommon.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11MeshRenderer.hpp"


class D3D11Mesh;
class D3D11Material;

class D3D11Model {
public:
	//STRUCTORS
	D3D11Model();
	D3D11Model(const Vector3& position);
	~D3D11Model();

	void SetMaterial(const String& matName);

	void Render(D3D11MeshRenderer* customMeshRenderer = nullptr) const;
	void Render(D3D11Material* matToRenderWith) const;

	const Matrix44* GetModel() { return &m_model; }

public:
	D3D11MeshRenderer*	m_meshRenderer	= nullptr;
	D3D11Mesh*			m_mesh			= nullptr;
	D3D11Material*		m_material		= nullptr;
	String				m_materialName	= "NULL";
	Matrix44			m_model			= Matrix44::IDENTITY;

	D3D11Model*			m_prev			= nullptr;
	D3D11Model*			m_next			= nullptr;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Model::Render(D3D11MeshRenderer* customMeshRenderer) const {

	if (customMeshRenderer) {
		customMeshRenderer->RenderMeshWithMaterial(m_mesh, m_material);
	}
	else {
		m_meshRenderer->RenderMeshWithMaterial(m_mesh, m_material);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Model::Render(D3D11Material* matToRenderWith) const {

	m_meshRenderer->RenderMeshWithMaterial(m_mesh, matToRenderWith);
}