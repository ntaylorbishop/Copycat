#include "Engine/Renderer/Effects/OIT/OIT.hpp"
#include "Engine/Renderer/Structures/SSBO.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/GPUPerformanceTimer.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Renderer/General/TextureCubemap.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"

STATIC OIT* OIT::s_oitPass = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PASS RUNNING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void OIT::RenderTransparentElements_Debug(Scene* sceneToRender, TextureCubemap* colorTarget, uint whichSide) {

	BeirusRenderer::DisableDepthWriting();
	BeirusRenderer::SetDepthTestingMode(GL_LESS);
	BeirusRenderer::EnableDepthTesting();

	BeirusRenderer::EnableBlending();

	BeirusRenderer::BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	sceneToRender->Render(true);

	BeirusRenderer::DisableBlending();
}


//---------------------------------------------------------------------------------------------------------------------------
void OIT::RenderTransparentElementsForBlending(Scene* sceneToRender, TextureCubemap* colorTarget, GLenum whichSide) {

	if (m_debugBlending) {
		RenderTransparentElements_Debug(sceneToRender, colorTarget, whichSide);
		return;
	}


	m_accumTarget->Bind();

	float accumClearVal[] = { 0.f, 0.f, 0.f, 0.f };
	float revealageClearVal = 1.f;

	BeirusRenderer::ClearColorTarget(GL_COLOR, 0, accumClearVal);
	BeirusRenderer::ClearColorTarget(GL_COLOR, 1, &revealageClearVal);

	BeirusRenderer::DisableDepthWriting();
	BeirusRenderer::SetDepthTestingMode(GL_LESS);
	BeirusRenderer::EnableDepthTesting();


	//Enable separate blending on each color target
	BeirusRenderer::EnableBlendingForColorTarget(0);
	BeirusRenderer::EnableBlendingForColorTarget(1);

	BeirusRenderer::SetBlendModeForColorTarget(GL_ONE, GL_ONE, 0);
	BeirusRenderer::SetBlendModeForColorTarget(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, 1);

	sceneToRender->Render(true);

	BeirusRenderer::DisableBlendingForColorTarget(0);
	BeirusRenderer::DisableBlendingForColorTarget(1);

	BlendColorTargetsForOutput(colorTarget, whichSide);
}




//---------------------------------------------------------------------------------------------------------------------------
void OIT::BlendColorTargetsForOutput(TextureCubemap* colorTarget, GLenum whichSide) {

	m_compositingTarget->Bind();

	if (colorTarget) {
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, whichSide, colorTarget->GetTextureID(), 0));
	}
	else {
		TextureBuffer* defaultColorTarget = BeirusRenderer::GetDefaultColorTarget();
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, defaultColorTarget->GetTextureID(), 0));
	}

	BeirusRenderer::EnableBlending();
	BeirusRenderer::BlendMode(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);


	BeirusRenderer::DisableBackfaceCulling();
	BeirusRenderer::DisableDepthTesting();

	MeshID fsMesh		= BeirusRenderer::GetGlobalFullscreenMesh();
	MeshRenderer* mr	= BeirusRenderer::GetGlobalMeshRenderer();


	mr->RenderMeshWithMaterial(fsMesh, m_compositingMat, Matrix4::IDENTITY);

	BeirusRenderer::DisableBlending();
	BeirusRenderer::EnableDepthTesting();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC OIT* OIT::Get() {

	if (s_oitPass == nullptr) {
		s_oitPass = new OIT();
		BeirusEngine::RegisterShutdownCallback(&OIT::Shutdown);
	}
	return s_oitPass;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEBUG
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void OIT::DebugDrawAccumBuffer(const AABB2& quadToDrawOn) {

	if (s_oitPass->m_debugAccumBuffer == nullptr) {
		s_oitPass->m_debugAccumBuffer = m_accumBuffer->AsTexture();
		s_oitPass->m_debugAccumBuffer->SetName("AccumBuffer");
	}

	BeirusRenderer::DrawTexturedAABB2(s_oitPass->m_debugAccumBuffer, AABB2(Vector2(0.f, 1.f), Vector2(1.f, 0.f)), RGBA::WHITE, quadToDrawOn);
}


//---------------------------------------------------------------------------------------------------------------------------
void OIT::DebugDrawRevealageBuffer(const AABB2& quadToDrawOn) {

	if (s_oitPass->m_debugRevealageBuffer == nullptr) {
		s_oitPass->m_debugRevealageBuffer = m_revealageBuffer->AsTexture();
		s_oitPass->m_debugRevealageBuffer->SetName("RevealageBuffer");
	}

	//DRAW
	Texture* rBuffer = s_oitPass->m_debugRevealageBuffer;
	Material* matToUse = BeirusRenderer::GetOrGenerateOneChannelMaterial(rBuffer);	

	StaticMesh2D newQuad;
	std::vector<Vertex2D_PCT> verts;

	AABB2 texCoords = AABB2(Vector2(0.f, 1.f), Vector2(1.f, 0.f));

	verts.push_back(Vertex2D_PCT(Vector2(quadToDrawOn.mins.x, quadToDrawOn.mins.y), RGBA::WHITE, Vector2(texCoords.mins.x, texCoords.maxs.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quadToDrawOn.maxs.x, quadToDrawOn.mins.y), RGBA::WHITE, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quadToDrawOn.maxs.x, quadToDrawOn.maxs.y), RGBA::WHITE, Vector2(texCoords.maxs.x, texCoords.mins.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quadToDrawOn.mins.x, quadToDrawOn.maxs.y), RGBA::WHITE, Vector2(texCoords.mins.x, texCoords.mins.y)));

	newQuad.m_verts = verts;
	newQuad.m_material = matToUse;


	BeirusRenderer::DisableDepthTesting();

	MeshRenderer meshRenderer;

	MeshID smID = BeirusRenderer::GetScratch2DMeshID();
	Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(smID);
	mesh->UpdateMesh(verts, newQuad.m_meshIndices);
	meshRenderer.RenderMeshWithMaterial(smID, matToUse, Matrix4::IDENTITY);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
OIT::OIT() {

	//INIT ACCUM TARGET
	m_revealageBuffer	= new TextureBuffer(SCREEN_SIZE_INT.x, SCREEN_SIZE_INT.y, TEXTURE_FORMAT_R32F);
	m_accumBuffer		= new TextureBuffer(SCREEN_SIZE_INT.x, SCREEN_SIZE_INT.y, TEXTURE_FORMAT_RGBA16F);

	TextureBuffer* depthTarget = BeirusRenderer::GetGlobalDepthMap();

	std::vector<TextureBuffer*> accumTargets;

	accumTargets.push_back(m_accumBuffer);
	accumTargets.push_back(m_revealageBuffer);

	m_accumTarget		= new Framebuffer(accumTargets, depthTarget);

	//INIT COMPOSITING TARGET
	TextureBuffer* defaultColorTarget = BeirusRenderer::GetDefaultColorTarget();

	std::vector<TextureBuffer*> compositingTargets;
	
	compositingTargets.push_back(defaultColorTarget);
	
	m_compositingTarget = new Framebuffer(compositingTargets, nullptr);

	m_compositingMat = MaterialBank::GetMaterial("OITBlend");
	m_compositingMat->m_renderState.m_backfaceCulling = false;
	ShaderProgram* oitProg = ShaderProgram::CreateOrGetShaderProgram("OITBlend");

	Uniform* revealageUni	= oitProg->GetUniform("gRevealageTex");
	Uniform* accumUni		= oitProg->GetUniform("gAccumTex");

	revealageUni->SetData(m_revealageBuffer);
	accumUni->SetData(m_accumBuffer);
}


//---------------------------------------------------------------------------------------------------------------------------
OIT::~OIT() {

	delete m_compositingTarget;
	delete m_accumTarget;
	delete m_revealageBuffer;
	delete m_accumBuffer;

	m_compositingTarget = nullptr;
	m_accumTarget		= nullptr;
	m_revealageBuffer	= nullptr;
	m_accumBuffer		= nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void OIT::Shutdown() {

	delete s_oitPass;
	s_oitPass = nullptr;
}