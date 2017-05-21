#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/AABB2.hpp"

class SSBO;
class Scene;
class Framebuffer;
class TextureBuffer;
class TextureCubemap;
class Material;
class Texture;

const uint NUM_PIXELS_IN_BUFFER = 4;
const uint PIXEL_SIZE_BYTES = sizeof(float) * 4 + sizeof(double);


struct Pixel {
	float m_redChannel		= 0.f;
	float m_greenChannel	= 0.f;
	float m_blueChannel		= 0.f;
	float m_alphaChannel	= 0.f;
	double m_depth			= 1.f;
};


class OIT {
public:
	//Renders all transparent elements in the scene to one of 5 separate color targets depending on depth,
	//And writes its depth to the depth SSBO. The depth SSBO holds depth for each pixel for lookup to determine
	//what color target the 
	void RenderTransparentElements_Debug(Scene* sceneToRender, TextureCubemap* colorTarget = nullptr, uint whichSide = 0);
	void RenderTransparentElementsForBlending(Scene* sceneToRender, TextureCubemap* colorTarget = nullptr,
		uint whichSide = 0);


	//Clamps down all framebuffers into colorTargetBack
	void BlendColorTargetsForOutput(TextureCubemap* colorTarget = nullptr, uint whichSide = 0);

	static OIT* Get();

	//DEBUG
	void ToggleDebug()	{ m_debugBlending = !m_debugBlending;	}
	void EnableDebug()	{ m_debugBlending = true;				}
	void DisableDebug() { m_debugBlending = false;				}

	void DebugDrawAccumBuffer(const AABB2& quadToDrawOn);
	void DebugDrawRevealageBuffer(const AABB2& quadToDrawOn);

private:
	//STRUCTORS INIT
	OIT();
	~OIT();

	static void Shutdown();

	Framebuffer*	m_compositingTarget		= nullptr;
	Framebuffer*	m_accumTarget			= nullptr;

	Texture*		m_debugRevealageBuffer	= nullptr;
	Texture*		m_debugAccumBuffer		= nullptr;

	TextureBuffer*	m_revealageBuffer		= nullptr;
	TextureBuffer*	m_accumBuffer			= nullptr;
	Material*		m_compositingMat		= nullptr;

	bool			m_debugBlending			= false;



	static OIT* s_oitPass;
};