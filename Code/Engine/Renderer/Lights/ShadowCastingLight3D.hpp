#pragma once

#include "Engine/Math/Matrix4.hpp"
#include "Engine/Renderer/Lights/Light3D.hpp"

class Scene;
class TextureCubemap;
class TextureBuffer;
class Framebuffer;
class Material;

const uint DEPTH_BUFFER_RESOLUTION = 1024;

class ShadowCastingLight3D : public Light3D {
public:
	//STRUCTORS
	ShadowCastingLight3D(const Vector3& pos, const RGBA& color, float radius);
	VIRTUAL ~ShadowCastingLight3D();


	//UPDATE
	VIRTUAL void RenderShadowMaps(Scene* scene);


	//GETTERS SETTERS
	virtual bool IsShadowCasting()		{ return true;				}
	TextureCubemap* GetShadowMap()		{ return m_depthMap;		}
	void SetToMoving(bool isMoving)		{ m_isMoving = isMoving;	}

private:
	void HookUpShadowMapToShader(const char* shaderName);
	void RenderShadowMap(Scene* scene, uint whichSide, const Matrix4& currProj);

	//INHERITED
	//Vector3	m_position;
	//RGBA		m_color;
	//float		m_minLightDistance;
	//float		m_maxLightDistance;
	//float		m_powerAtMin;
	//float		m_powerAtMax;

	TextureCubemap* m_depthMap				= nullptr;

	Matrix4			m_forwardOrtho			= Matrix4::IDENTITY;
	Matrix4			m_backwardOrtho			= Matrix4::IDENTITY;
	Matrix4			m_leftOrtho				= Matrix4::IDENTITY;
	Matrix4			m_rightOrtho			= Matrix4::IDENTITY;
	Matrix4			m_topOrtho				= Matrix4::IDENTITY;
	Matrix4			m_bottomOrtho			= Matrix4::IDENTITY;

	float			m_nearPlane				= 1.f;
	float			m_farPlane				= 100.f;
	uint			m_framebufferID			= 0;
	uint8_t			m_sideToRenderThisFrame = 0;
	bool			m_isMoving				= false;

	static Material* s_depthOnlyMat;
};