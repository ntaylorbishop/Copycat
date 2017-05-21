#pragma once

#define _USE_MATH_DEFINES

#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Lights/LightSystem.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"

const int DEFAULT_SPHERE_FIDELITY = 100;

typedef void(RenderSetupCallback)(void*);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATICMESH CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Model {
public:
	//STRUCTORS
	Model();
	Model(const Vector3& position, ePrimitiveMeshType meshType = PRIMITIVE_MESH_CUBE);
	~Model();

	void SetMaterial(const String& matName);
	void ChangeColor(const RGBA& color);

	//UPDATE RENDER
	void Render(MeshRenderer* customMeshRenderer = nullptr) const;
	void Render(Material* matToRenderWith) const;

	float CalcDistFromCamera() const;

	//GETTERS SETTERS
	bool GetCastsShadows();
	void SetShadowCaster(bool castsShadows);
	void SetRenderSetupCallback(RenderSetupCallback* rsCallback, void* cbUserData);
	bool IsActive() const																{ return m_isActive;	}
	void Disable()																		{ m_isActive = false;	}
	void Enable()																		{ m_isActive = true;	}
	const Matrix4* GetModel()															{ return &m_model;		}

	bool operator==(const Model& other);
	bool operator!=(const Model& other);


public:
	MeshRenderer*			m_meshRenderer		= nullptr;
	MeshID					m_meshID			= 0;
	Material*				m_material			= nullptr;
	String					m_materialName		= "NULL";
	RGBA					m_color				= RGBA::WHITE;
	Model*					m_prev				= nullptr;
	Model*					m_next				= nullptr;
	bool					m_isActive			= true;

	RenderSetupCallback*	m_rsCallback		= nullptr;
	void*					m_callbackUserData	= nullptr;

	RenderState				m_renderState;

	Matrix4					m_model				= Matrix4::IDENTITY;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline void Model::Render(MeshRenderer* customMeshRenderer) const {
	if (m_rsCallback != nullptr) {
		m_rsCallback(m_callbackUserData);
	}

	if (customMeshRenderer == nullptr) {
		m_meshRenderer->RenderMeshWithMaterial(m_meshID, m_material, m_model);
	}
	else {
		customMeshRenderer->RenderMeshWithMaterial(m_meshID, m_material, m_model);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Model::Render(Material* matToRenderWith) const {


	m_meshRenderer->RenderMeshWithMaterial(m_meshID, matToRenderWith, m_model);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Model::SetShadowCaster(bool castsShadows) {
	if (castsShadows) {
		m_renderState.m_castsShadows = CASTS_SHADOWS_ON;
	}
	else {
		m_renderState.m_castsShadows = CASTS_SHADOWS_OFF;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Model::SetRenderSetupCallback(RenderSetupCallback* rsCallback, void* cbUserData) {

	m_callbackUserData	= cbUserData;
	m_rsCallback		= rsCallback;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OPERATORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline bool Model::operator==(const Model& other) {
	return m_meshRenderer->m_vaoID == other.m_meshRenderer->m_vaoID;
}


//---------------------------------------------------------------------------------------------------------------------------
inline bool Model::operator!=(const Model& other) {
	return m_meshRenderer->m_vaoID != other.m_meshRenderer->m_vaoID;
}