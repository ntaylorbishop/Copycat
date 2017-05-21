#include "Engine/Renderer/Effects/SSAO.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"


STATIC SSAO* SSAO::s_ssao = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void SSAO::RenderSSAOMap() {
	
	if (!s_ssao) {
		InitializeRandomSeedForRand();
		s_ssao = new SSAO();
	}
	
	ShaderProgram* ssaoProg = ShaderProgram::CreateOrGetShaderProgram("SSAO");
	ssaoProg->GetUniform("gSceneView")	->SetData((void*)BeirusRenderer::GetViewMatrix());
	ssaoProg->GetUniform("gSceneProj")	->SetData((void*)BeirusRenderer::GetProjectionMatrix());
	ssaoProg->GetUniform("gKernel")		->SetData((void*)s_ssao->m_kernels.data());
	ssaoProg->GetUniform("gDebugDraw")	->SetData((void*)&s_ssao->m_debugEnable);

	
	s_ssao->m_fbo->Bind();
	BeirusRenderer::ClearScreen(RGBA(0.1f, 0.1f, 0.1f, 1.f));
	
	BeirusRenderer::DisableDepthTesting();
	BeirusRenderer::DisableDepthWriting();
	BeirusRenderer::DisableBackfaceCulling();
	
	MeshRenderer* mr	= BeirusRenderer::GetGlobalMeshRenderer();
	MeshID mesh			= s_ssao->m_fullScreenMesh;
	Material* mat		= s_ssao->m_material;

	mr->RenderMeshWithMaterial(mesh, mat, Matrix4::IDENTITY);

	s_ssao->m_blurFBO->Bind();
	mr->RenderMeshWithMaterial(mesh, s_ssao->m_blurMat, Matrix4::IDENTITY);
	
	BeirusRenderer::EnableDepthTesting();
	BeirusRenderer::EnableDepthWriting();
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC TextureBuffer* SSAO::GetSSAOMap() {

	if (!s_ssao) {
		InitializeRandomSeedForRand();
		s_ssao = new SSAO();
	}
	
	return s_ssao->m_blurredSSAO;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE INITIALIZE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
SSAO::SSAO() {

	TextureBuffer* depthMap = BeirusRenderer::GetGlobalDepthMap();

	const Vector2* screenSize = BeirusRenderer::GetScreenSize();
	m_ssaoMap = new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_RGBA8);

	std::vector<TextureBuffer*> colorTargets;
	colorTargets.push_back(m_ssaoMap);
	m_fbo = new Framebuffer(colorTargets, nullptr);

	m_randomTexture = Texture::CreateOrGetTexture("Data/Textures/SSAO_RandomizedNormals.png");
	m_fullScreenMesh = BeirusRenderer::GetGlobalFullscreenMesh();

	GenerateKernels();
	InitalizeSSAOShaderProgramAndMaterial(depthMap);



	//SET UP BLUR PASS
	ShaderProgram* blurProg = ShaderProgram::CreateOrGetShaderProgram("SSAOBlur");
	blurProg->CreateUniform("gTexSSAO", UNIFORM_TEXTUREBUFFER2D, 1, 0, m_ssaoMap);
	m_blurMat = new Material("SSAOBlur");
	m_blurMat->m_renderState.m_backfaceCulling = false;

	m_blurredSSAO = new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_RGBA8);
	std::vector<TextureBuffer*> blurTarget;
	blurTarget.push_back(m_blurredSSAO);
	m_blurFBO = new Framebuffer(blurTarget, nullptr);

	ShaderProgram* blinnProg = ShaderProgram::CreateOrGetShaderProgram("BlinnPhong");
	Uniform* ssaoUni = blinnProg->GetUniform("gTexSSAO");
	ssaoUni->SetData(m_blurredSSAO);
}


//---------------------------------------------------------------------------------------------------------------------------
SSAO::~SSAO() {
	delete m_ssaoMap;
	m_ssaoMap = nullptr;
	delete m_fbo;
	m_fbo = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void SSAO::GenerateKernels() {

	//GENERATE SAMPLE KERNEL

	for (int i = 0; i < KERNEL_SIZE; ++i) {

		Vector3 kernel = Vector3(Randf(-1.f, 1.f), Randf(-1.f, 1.f), Randf(0.f, 1.f));
		kernel.Normalize();

		kernel *= Randf(0.f, 1.f);

		//Scale current kernel with logarithmic falloff
		float scale = (float)i / (float)KERNEL_SIZE;
		scale = Lerp(0.1f, 1.f, scale * scale);
		kernel *= scale;

		m_kernels.push_back(kernel);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void SSAO::InitalizeSSAOShaderProgramAndMaterial(TextureBuffer* depthMap) {

	UNUSED(depthMap);

	ShaderProgram* ssaoProg = ShaderProgram::CreateOrGetShaderProgram("SSAO");
	ssaoProg->CreateUniform("gKernel",			UNIFORM_VECTOR3,			m_kernels.size(), m_kernels.data());

	Uniform* randTexUni = ssaoProg->GetUniform("gTexRandom");
	randTexUni->SetData(m_randomTexture);

	m_material = new Material("SSAO");
	m_material->m_renderState.m_backfaceCulling = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEBUG
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void SSAO::DebugDrawSSAOMapBlur(const AABB2& quadToDrawOn) {

	if (s_ssao->m_debugMapBlur == nullptr) {
		s_ssao->m_debugMapBlur = s_ssao->m_blurredSSAO->AsTexture();
		s_ssao->m_debugMapBlur->SetName("SSAOMapBlur");
	}

	BeirusRenderer::DrawTexturedAABB2(s_ssao->m_debugMapBlur, AABB2(Vector2(0.f, 1.f), Vector2(1.f, 0.f)), RGBA::WHITE, quadToDrawOn);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void SSAO::DebugDrawNormalsMap(const AABB2& quadToDrawOn) {

	TextureBuffer* normals = BeirusRenderer::GetGlobalNormalsMap();

	if (s_ssao->m_debugNormalMap == nullptr) {
		s_ssao->m_debugNormalMap = normals->AsTexture();
		s_ssao->m_debugNormalMap->SetName("GlobalNormalsMap");
	}

	BeirusRenderer::DrawTexturedAABB2(s_ssao->m_debugNormalMap, AABB2(Vector2(0.f, 1.f), Vector2(1.f, 0.f)), RGBA::WHITE, quadToDrawOn);
}

//---------------------------------------------------------------------------------------------------------------------------
STATIC void SSAO::DebugDrawSSAOMapNonBlur(const AABB2& quadToDrawOn) {

	if (s_ssao->m_debugMapNonBlur == nullptr) {
		s_ssao->m_debugMapNonBlur = s_ssao->m_ssaoMap->AsTexture();
		s_ssao->m_debugMapNonBlur->SetName("SSAOMapNonBlur");
	}

	BeirusRenderer::DrawTexturedAABB2(s_ssao->m_debugMapNonBlur, AABB2(Vector2(0.f, 1.f), Vector2(1.f, 0.f)), RGBA::WHITE, quadToDrawOn);
}