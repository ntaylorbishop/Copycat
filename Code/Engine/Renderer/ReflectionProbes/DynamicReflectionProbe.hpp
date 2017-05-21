#pragma once

#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Renderer/RendererCommon.hpp"
#include "Engine/Scene/ClickableObject.hpp"

class Material;
class Scene;
class TextureCubemap;

const uint REFLECTION_TEX_SIZE = 512;


class DynamicReflectionProbe : public ClickableObject {
public:
	//STRUCTORS
	DynamicReflectionProbe(const Vector3& position, Scene* sceneToAddTo, uint texSize);
	~DynamicReflectionProbe();

	static DynamicReflectionProbe* Create(const Vector3& position, Scene* sceneToAddTo, uint texSize = 512);

	//UPDATE RENDER

	void Update(Scene* sceneToRender);
	void RenderSide(Scene* scene, uint whichSide);

	void Render() const;

	//GETTERS SETTERS
	void SetPosition(const Vector3& newPos);
	void EnableDebugDraw()											{ m_isDebugDrawEnabled = true;					}
	void DisableDebugDraw()											{ m_isDebugDrawEnabled = false;					}
	TextureCubemap* GetReflectionMap()								{ return m_colorTarget;							}
	void WriteToFile(const String& path, const String& namePrefix);



	Vector3			m_position				= Vector3::ZERO;
	Vector3			m_boxMins				= Vector3::ZERO;
	Vector3			m_boxMaxs				= Vector3::ZERO;
	float			m_radius				= 10.f;

private:
	//CLICKABLES
	void UpdateAxisBoundingBoxes();
	void RenderAxes() const;

	AABB3 m_xAxis;
	AABB3 m_yAxis;
	AABB3 m_zAxis;


	TextureCubemap* m_colorTarget			= nullptr;
	TextureCubemap* m_depthTarget			= nullptr;
	Matrix4			m_proj					= Matrix4::IDENTITY;
	Matrix4			m_view					= Matrix4::IDENTITY;

	Vector3			m_forwardRot			= Vector3(-720.f, -180.f,    0.f);
	Vector3			m_backwardRot			= Vector3( 540.f,  540.f,  180.f);
	Vector3			m_leftRot				= Vector3( 450.f,    0.f,  180.f);
	Vector3			m_rightRot				= Vector3( 630.f, -450.f,  -90.f);
	Vector3			m_topRot				= Vector3( 180.f,  450.f, -180.f);
	Vector3			m_bottomRot				= Vector3(   0.f,  450.f,    0.f);

	bool			m_isDebugDrawEnabled	= true;
	float			m_nearPlane				= 0.1f;
	float			m_farPlane				= 1000.f;
	uint8_t			m_sideToRenderThisFrame = 0;
	uint			m_framebufferID			= 0;
	Model*			m_model					= nullptr;
	Material*		m_material				= nullptr;

	MeshRenderer    m_meshRendererWithCustomMats;
};