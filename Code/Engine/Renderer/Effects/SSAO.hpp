#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"

class TextureBuffer;
class Framebuffer;
class Texture;
class Mesh;
class Material;

const int KERNEL_SIZE = 16;

class SSAO {
public:
	static void RenderSSAOMap();
	static TextureBuffer* GetSSAOMap();

	static void DebugDrawSSAOMapBlur(const AABB2& quadToDrawOn);
	static void DebugDrawNormalsMap(const AABB2& quadToDrawOn);
	static void DebugDrawSSAOMapNonBlur(const AABB2& quadToDrawOn);

	static void	ToggleDebugEnable()		{ s_ssao->m_debugEnable = !s_ssao->m_debugEnable;	}
	static void EnableDebugEnable()		{ s_ssao->m_debugEnable = true;						}
	static void DisableDebugEnable()	{ s_ssao->m_debugEnable = false;					}
	static bool	GetDebugEnable()		{ return s_ssao->m_debugEnable;						}

private:
	//STRUCTORS
	SSAO();
	~SSAO();

	void GenerateKernels();

	void InitalizeSSAOShaderProgramAndMaterial(TextureBuffer* depthMap);

	Texture*		m_debugNormalMap	= nullptr;
	Texture*		m_debugMapBlur		= nullptr;
	Texture*		m_debugMapNonBlur	= nullptr;
	TextureBuffer*	m_ssaoMap			= nullptr;
	Framebuffer*	m_fbo				= nullptr;
	Texture*		m_randomTexture		= nullptr;
	MeshID			m_fullScreenMesh	= 0;
	Material*		m_material			= nullptr;
	std::vector<Vector3> m_kernels;

	Framebuffer*	m_blurFBO			= nullptr;
	Material*		m_blurMat			= nullptr;
	TextureBuffer*	m_blurredSSAO		= nullptr;
	bool			m_debugEnable		= true;

	static SSAO*	s_ssao;
};