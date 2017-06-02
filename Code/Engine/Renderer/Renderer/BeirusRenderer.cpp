#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Renderer/GlobalStateManager.hpp"
#include "Engine/Renderer/Mesh/Model.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Shaders/ShaderProgramParser.hpp"
#include "Engine/Renderer/Shaders/ComputeShaderProgram.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"
#include "Engine/Renderer/Structures/SSBO.hpp"
#include "Engine/Renderer/Effects/SSAO.hpp"
#include "Engine/Renderer/Effects/DepthAndNormalsPass.hpp"
#include "Engine/Renderer/Cameras/PlayerCamera3D.hpp"
#include "Engine/Renderer/Cameras/Camera2D.hpp"
#include "Engine/Renderer/Lights/LightSystem.hpp"
#include "Engine/Renderer/OpenGL/OpenGLErrorReporter.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/GPUPerformanceTimer.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Renderer/Effects/OIT/OIT.hpp"
#include "Engine/Renderer/ReflectionProbes/DynamicReflectionProbeManager.hpp"
#include "Engine/Renderer/Effects/Tonemapping.hpp"

#pragma warning(push, 1)
#include "ThirdParty/FastDelegate.h"
#pragma warning(pop)
using namespace fastdelegate;


STATIC BeirusRenderer*	BeirusRenderer::s_theBeirusRenderer = nullptr;
STATIC MeshID			BeirusRenderer::s_fullScreenMesh	= 0;
STATIC MeshID			BeirusRenderer::s_scratchMesh2D		= 0;
STATIC MeshID			BeirusRenderer::s_scratchLineMesh	= 0;

Matrix4 g_defaultPerspectiveProjectionMatrix	= Matrix4::IDENTITY;
Matrix4 g_defaultUIMatrix						= Matrix4::IDENTITY;


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::InitializeAllShaders() {

	std::vector<String> filenames = FileUtils::EnumerateFiles("Data/Shaders/", "*.xml", true);

	for (uint shaderFileIdx = 0; shaderFileIdx < filenames.size(); shaderFileIdx++) {
		ShaderProgramParser::ParseShaderProgramsFromXML("Data/Shaders/", filenames[shaderFileIdx].c_str());
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT SHUTDOWN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC void BeirusRenderer::Initialize(const RGBA& clearColor) {

	GL_CHECK(glEnable(GL_DEBUG_OUTPUT));

	ASSERT_OR_DIE(s_theBeirusRenderer == nullptr, "ERROR: BeirusRenderer already initialized.");
	InitializeOpenGLDefinitions();
	s_theBeirusRenderer = new BeirusRenderer(clearColor);
	BeirusEngine::RegisterUpdateCallback(MakeDelegate(s_theBeirusRenderer, &BeirusRenderer::Update));

	#if IS_WINDOWED
	s_theBeirusRenderer->m_screenSize = Vector2(1600.f, 900.f);
	#else
	s_theBeirusRenderer->m_screenSize = Vector2(3840.f, 2160.f);
	#endif

	GL_CHECK(glFrontFace(GL_CW));

	s_theBeirusRenderer->InitializeAllShaders();

	s_theBeirusRenderer->m_fxaaMat				= MaterialBank::GetMaterial("FXAA");
	s_theBeirusRenderer->m_fxaaMat->m_renderState.m_backfaceCulling				= false;


	Material* dnMat = MaterialBank::GetMaterial("DepthAndNormalsPass");

	dnMat->CreateUniform("gTexNormal", UNIFORM_TEXTURE2D, 1, 4);

	OIT::Get();

	Material* lineMat = MaterialBank::GetMaterial("");
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::InitDefaultProjectionMats() {

	//DEFAULT PERSPECTIVE
	g_defaultPerspectiveProjectionMatrix.ChangeToPerspectiveProjection(DEFAULT_PERSPECTIVE_FOV,
		DEFAULT_ASPECT_RATIO, DEFAULT_NEAR_PLANE, DEFAULT_FAR_PLANE);
	g_defaultUIMatrix.ChangeToProjectionMatrix(0, 1920.f, 0.f, 1080.f, -1.f, 100.f);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::Shutdown() {

	ASSERT_OR_DIE(s_theBeirusRenderer != nullptr, "ERROR: BeirusRenderer already shutdown.");

	if (s_theBeirusRenderer->m_playerCamera) {
		PlayerCamera3D::Destroy();
	}

	delete s_theBeirusRenderer;
	s_theBeirusRenderer = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATE SWITCHING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::EnableBackfaceCulling() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleBackfaceCulling(true)) {
		GL_CHECK(glEnable(GL_CULL_FACE));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::EnableBlending() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleBlending(true)) {
		GL_CHECK(glEnable(GL_BLEND));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::ClearColorTarget(GLenum targetType, int idx, float* val) {

	GL_CHECK(glClearBufferfv(targetType, idx, val));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::EnableBlendingForColorTarget(uint colorTargetIdx) {
	GL_CHECK(glEnablei(GL_BLEND, colorTargetIdx));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DisableBlendingForColorTarget(uint colorTargetIdx) {
	GL_CHECK(glDisablei(GL_BLEND, colorTargetIdx));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::SetBlendModeForColorTarget(GLenum src, GLenum dest, uint colorTargetIdx) {
	GL_CHECK(glBlendFunci(colorTargetIdx, src, dest));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::EnableDepthTesting() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleDepthTesting(true)) {
		GL_CHECK(glEnable(GL_DEPTH_TEST));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::EnableLineSmoothing() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleLineSmoothing(true)) {
		GL_CHECK(glEnable(GL_LINE_SMOOTH));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::EnableDepthWriting() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleDepthWriting(true)) {
		glDepthMask(GL_TRUE);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::EnableColorWriting() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleColorWriting(true)) {
		GL_CHECK(glColorMask(1, 1, 1, 1));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DisableColorWriting() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleColorWriting(false)) {
		GL_CHECK(glColorMask(0, 0, 0, 0));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DisableDepthWriting() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleDepthWriting(false)) {
		GL_CHECK(glDepthMask(GL_FALSE));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DisableBackfaceCulling() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleBackfaceCulling(false)) {
		GL_CHECK(glDisable(GL_CULL_FACE));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DisableBlending() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleBlending(false)) {
		GL_CHECK(glDisable(GL_BLEND));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DisableDepthTesting() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleDepthTesting(false)) {
		GL_CHECK(glDisable(GL_DEPTH_TEST));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DisableLineSmoothing() {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldToggleLineSmoothing(false)) {
		GL_CHECK(glDisable(GL_LINE_SMOOTH));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::SetDepthTestingMode(GLenum depthTestMode) {
	GL_CHECK(glDepthFunc(depthTestMode));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::SetCullFace(bool isFront) {

	if (GlobalStateManager::GetGlobalStateManager()->ShouldChangeCullFace(isFront)) {

		if (isFront) {
			GL_CHECK(glCullFace(GL_FRONT));
		}
		else {
			GL_CHECK(glCullFace(GL_BACK));
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::BlendMode(GLenum sfactor, GLenum dfactor) {
	GL_CHECK(glBlendFunc(sfactor, dfactor));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::PointSize(float size) {
	GL_CHECK(glPointSize(size));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::LineWidth(float width) {
	GL_CHECK(glLineWidth(width));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::ClearBuffer(GLbitfield mask) {
	GL_CHECK(glClear(mask));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::ClearScreen(const RGBA& col) {
	if (GlobalStateManager::GetGlobalStateManager()->ShouldChangeClearColor(col)) {
		GL_CHECK(glClearColor(col.r, col.g, col.b, 1.f));
	}
	if (GlobalStateManager::GetGlobalStateManager()->ShouldChangeClearDepthNumber(1.f)) {
		GL_CHECK(glClearDepth(1.f));
	}
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::ClearScreen() {
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::ClearDepth() {
	GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::ClearColor() {
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC String BeirusRenderer::GetCurrentVersionOfGLSL() {
	String glShadingVersion = reinterpret_cast< char const * >(glGetString(GL_SHADING_LANGUAGE_VERSION));
	return glShadingVersion;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC String BeirusRenderer::GetCurrentVersionOfOpenGL() {
	String glShadingVersion = reinterpret_cast<char const *>(glGetString(GL_VERSION));
	return glShadingVersion;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2D DRAWING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC Material* BeirusRenderer::GetOrGenerateMaterial(Texture* tex) {

	String texName = tex->GetName();
	Material* mat = MaterialBank::GetMaterial(texName);

	if (!mat) {
		mat = new Material("Default2D_NoAlpha");
		mat->SetName(texName);
		mat->CreateUniform("gTexDiffuse", UNIFORM_TEXTURE2D, 1, 0, tex);
		MaterialBank::AddGeneratedMaterial(mat);
	}

	return mat;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Material* BeirusRenderer::GetOrGenerateOneChannelMaterial(Texture* tex) {

	String texName = tex->GetName();
	Material* mat = MaterialBank::GetMaterial(texName);

	if (!mat) {
		mat = new Material("Default2D_1Channel_NoAlpha");
		mat->SetName(texName);
		mat->CreateUniform("gTexDiffuse", UNIFORM_TEXTURE2D, 1, 0, tex);
		MaterialBank::AddGeneratedMaterial(mat);
	}

	return mat;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawAABB2(const AABB2& quad, const RGBA& tint) {

	StaticMesh2D newQuad;
	std::vector<Vertex2D_PCT> verts;

	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.mins.y), tint, Vector2(0.f, 0.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.mins.y), tint, Vector2(1.f, 0.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.maxs.y), tint, Vector2(1.f, 1.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.maxs.y), tint, Vector2(0.f, 1.f)));

	newQuad.m_verts = verts;
	newQuad.m_material = MaterialBank::GetMaterial("Default2D");
	Uniform* texUni = newQuad.m_material->GetUniform("gIsUsingTex");

	if (texUni == nullptr) {
		newQuad.m_material->CreateUniform("gIsUsingTex", UNIFORM_BOOL, 1, 0, &BOOL_FALSE);
	}
	else {
		texUni->SetData((void*)&BOOL_FALSE);
	}
	s_theBeirusRenderer->m_rQuadsToRender.push_back(newQuad);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawTexturedAABB2(Material* mat, const RGBA& tint, const AABB2& quad) {
	
	StaticMesh2D newQuad;
	std::vector<Vertex2D_PCT> verts;

	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.mins.y), tint, Vector2(0.f, 1.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.mins.y), tint, Vector2(1.f, 1.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.maxs.y), tint, Vector2(1.f, 0.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.maxs.y), tint, Vector2(0.f, 0.f)));

	newQuad.m_verts = verts;
	newQuad.m_material = mat;
	Uniform* texUni = newQuad.m_material->GetUniform("gIsUsingTex");

	if (texUni == nullptr) {
		newQuad.m_material->CreateUniform("gIsUsingTex", UNIFORM_BOOL, 1, 0, &BOOL_TRUE);
	}
	else {
		texUni->SetData((void*)&BOOL_TRUE);
	}

	s_theBeirusRenderer->m_rQuadsToRender.push_back(newQuad);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawTexturedAABB2(Material* mat, const AABB2& texCoords, const RGBA& tint, const AABB2& quad) {

	StaticMesh2D newQuad;
	std::vector<Vertex2D_PCT> verts;

	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.mins.y), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.mins.y), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.maxs.y), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.maxs.y), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));


	newQuad.m_verts = verts;
	newQuad.m_material = mat;

	s_theBeirusRenderer->m_rQuadsToRender.push_back(newQuad);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawTexturedAABB2(Texture* tex, const RGBA& tint, const AABB2& quad) {

	Material* matToUse = GetOrGenerateMaterial(tex);

	StaticMesh2D newQuad;
	std::vector<Vertex2D_PCT> verts;

	Vector2(0.f, 1.f);
	Vector2(1.f, 1.f);
	Vector2(1.f, 0.f);
	Vector2(0.f, 0.f);

	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.mins.y), tint, Vector2(1.f, 1.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.mins.y), tint, Vector2(1.f, 0.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.maxs.y), tint, Vector2(0.f, 0.f)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.maxs.y), tint, Vector2(0.f, 1.f)));

	newQuad.m_verts = verts;
	newQuad.m_material = matToUse;

	s_theBeirusRenderer->m_rQuadsToRender.push_back(newQuad);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawTexturedAABB2(Texture* tex, const AABB2& texCoords, const RGBA& tint, const AABB2& quad) {

	Material* matToUse = GetOrGenerateMaterial(tex);

	StaticMesh2D newQuad;
	std::vector<Vertex2D_PCT> verts;

	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.mins.y), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.mins.y), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.maxs.x, quad.maxs.y), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
	verts.push_back(Vertex2D_PCT(Vector2(quad.mins.x, quad.maxs.y), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));


	newQuad.m_verts = verts;
	newQuad.m_material = matToUse;



	s_theBeirusRenderer->m_rQuadsToRender.push_back(newQuad);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2D DRAWING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawCircle(const Camera2D& cam, const RGBA& color, const Vector3& position, const float radius) {

	LineMesh newLine;
	std::vector<TexturedVertex_TBNBN> verts;
	std::vector<uint16_t> inds;

	TexturedVertex_TBNBN v1(position, color, Vector2(0.f, 0.f), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, Vector4::ZERO, IntVector4::ZERO);
	TexturedVertex_TBNBN v2(position + Vector3(radius), color, Vector2(1.f, 1.f), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, Vector4::ZERO, IntVector4::ZERO);


	uint numPoints = 100;
	float degDiff = 360.f / (float)numPoints;
	float deg = 0.f;
	for (uint i = 0; i < numPoints; i++) {
		Vector3 p1 = position + Vector3(radius * CosDegrees(deg), radius * SinDegrees(deg), 0.f);
		TexturedVertex_TBNBN v1(p1, color, Vector2(0.f, 0.f), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, Vector4::ZERO, IntVector4::ZERO);
		verts.push_back(v1);

		deg += degDiff;

		Vector3 p2 = position + Vector3(radius * CosDegrees(deg), radius * SinDegrees(deg), 0.f);
		TexturedVertex_TBNBN v2(p2, color, Vector2(0.f, 0.f), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, Vector4::ZERO, IntVector4::ZERO);
		verts.push_back(v2);

		deg += degDiff;

		inds.push_back(i);
		inds.push_back(i + 1);
	}

	newLine.m_verts = verts;
	newLine.m_meshIndices = inds;

	Matrix4 viewMat = cam.GetViewMatrix();
	newLine.m_material->CreateUniform("gView", UNIFORM_MAT4, 1, &viewMat);
	newLine.m_material->CreateUniform("gProj", UNIFORM_MAT4, 1, &cam.m_proj);


	Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(s_scratchLineMesh);
	mesh->UpdateMesh(newLine.m_verts, newLine.m_meshIndices);
	s_theBeirusRenderer->m_meshRenderer->RenderMeshWithMaterial(s_scratchLineMesh, newLine.m_material, Matrix4::IDENTITY, true);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LINE DRAWING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawLine(const Vector3& start, const Vector3& end, const RGBA& tint) {

	LineMesh newLine;
	std::vector<TexturedVertex_TBNBN> verts;

	TexturedVertex_TBNBN v1(start,	tint, Vector2(0.f, 0.f), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, Vector4::ZERO, IntVector4::ZERO);
	TexturedVertex_TBNBN v2(end,	tint, Vector2(1.f, 1.f), Vector3::ZERO, Vector3::ZERO, Vector3::ZERO, Vector4::ZERO, IntVector4::ZERO);


	verts.push_back(v1);
	verts.push_back(v2);

	newLine.m_verts		= verts;
	s_theBeirusRenderer->m_linesToRender.push_back(newLine);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::DrawBoxLineMesh(const AABB3& box, const RGBA& tint) {

	DrawLine(Vector3(box.mins.x, box.mins.y, box.mins.z), Vector3(box.maxs.x, box.mins.y, box.mins.z), tint);
	DrawLine(Vector3(box.maxs.x, box.mins.y, box.mins.z), Vector3(box.maxs.x, box.maxs.y, box.mins.z), tint);
	DrawLine(Vector3(box.maxs.x, box.maxs.y, box.mins.z), Vector3(box.mins.x, box.maxs.y, box.mins.z), tint);
	DrawLine(Vector3(box.mins.x, box.maxs.y, box.mins.z), Vector3(box.mins.x, box.mins.y, box.mins.z), tint);

	DrawLine(Vector3(box.mins.x, box.mins.y, box.maxs.z), Vector3(box.maxs.x, box.mins.y, box.maxs.z), tint);
	DrawLine(Vector3(box.maxs.x, box.mins.y, box.maxs.z), Vector3(box.maxs.x, box.maxs.y, box.maxs.z), tint);
	DrawLine(Vector3(box.maxs.x, box.maxs.y, box.maxs.z), Vector3(box.mins.x, box.maxs.y, box.maxs.z), tint);
	DrawLine(Vector3(box.mins.x, box.maxs.y, box.maxs.z), Vector3(box.mins.x, box.mins.y, box.maxs.z), tint);

	DrawLine(Vector3(box.mins.x, box.mins.y, box.mins.z), Vector3(box.mins.x, box.mins.y, box.maxs.z), tint);
	DrawLine(Vector3(box.maxs.x, box.mins.y, box.mins.z), Vector3(box.maxs.x, box.mins.y, box.maxs.z), tint);
	DrawLine(Vector3(box.maxs.x, box.maxs.y, box.mins.z), Vector3(box.maxs.x, box.maxs.y, box.maxs.z), tint);
	DrawLine(Vector3(box.mins.x, box.maxs.y, box.mins.z), Vector3(box.mins.x, box.maxs.y, box.maxs.z), tint);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE MEMBER FUNCS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
BeirusRenderer::BeirusRenderer(const RGBA& clearColor) 
	: m_lightIndexBuffer(nullptr)
{

	MaterialBank::Initialize();
	LightSystem::Initialize();
	OpenGLErrorReporter::Initialize();

	VertexDefinition::InitializeVertexDefinitions();
	InitializeFBO();
	InitializeComputeShaderAndDataForIt();
	Texture* tex = Texture::CreateOrGetTexture("Data/Textures/DefaultMaterial.png");
	tex->GenerateMipmap();

	m_rQuadsToRender.reserve(MAX_NUM_RENDERABLE_QUADS);

	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	ClearScreen(clearColor);

	GL_CHECK(glEnable(GL_BLEND));
	GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//glCullFace(GL_FRONT);

	#if IS_WINDOWED
	GL_CHECK(glViewport(0, 0, (GLsizei)1600.f, (GLsizei)900.f));
	#else
	GL_CHECK(glViewport(0, 0, (GLsizei)3840.f, (GLsizei)2160.f));
	#endif

	m_gpuTimer	= new GPUPerformanceTimer();
}


//---------------------------------------------------------------------------------------------------------------------------
BeirusRenderer::~BeirusRenderer() {
	delete m_globalFBO;
	m_globalFBO = nullptr;
	delete m_meshRenderer;
	m_meshRenderer = nullptr;
}

uint GetNumTilesX() {

	const Vector2* screenSizeInPixels = BeirusRenderer::GetScreenSize();
	return ((uint)screenSizeInPixels->x + ((uint)screenSizeInPixels->x % TILE_SIZE_PIXELS)) / TILE_SIZE_PIXELS;
}

uint GetNumTilesY() {

	const Vector2* screenSizeInPixels = BeirusRenderer::GetScreenSize();
	return ((uint)screenSizeInPixels->y + ((uint)screenSizeInPixels->y % TILE_SIZE_PIXELS)) / TILE_SIZE_PIXELS;
}

//---------------------------------------------------------------------------------------------------------------------------
uint GetTotalNumberOfTiles() {

	const Vector2* screenSizeInPixels = BeirusRenderer::GetScreenSize();

	uint wNum = ((uint)screenSizeInPixels->x + ((uint)screenSizeInPixels->x % TILE_SIZE_PIXELS)) / TILE_SIZE_PIXELS;
	uint hNum = ((uint)screenSizeInPixels->y + ((uint)screenSizeInPixels->y % TILE_SIZE_PIXELS)) / TILE_SIZE_PIXELS;

	return wNum * hNum;
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::InitializeComputeShaderAndDataForIt() {

	uint totalNumTiles = GetTotalNumberOfTiles();

	m_lightIndexBuffer	= new SSBO();

	uint64_t totalSizeOfIndexBuffer	= MAX_NUM_LIGHTS_PER_TILE * totalNumTiles * sizeof(int32_t); //99KB
	m_lightIndexBuffer->Generate((size_t)totalSizeOfIndexBuffer);

	ComputeShaderProgram* comp = ComputeShaderProgram::CreateOrGetComputeShaderProgram("LightCulling");
	comp->AddSSBO(0, LightSystem::GetLightDataSSBO());
	comp->AddSSBO(1, m_lightIndexBuffer);

	uint* screenX = (uint*)&SCREEN_SIZE_INT.x;
	uint* screenY = (uint*)&SCREEN_SIZE_INT.y;
	TextureBuffer* depthMap = m_depthTarget;

	comp->CreateUniform("uProj",				UNIFORM_MAT4,				1, &Matrix4::IDENTITY);
	comp->CreateUniform("uView",				UNIFORM_MAT4,				1, &Matrix4::IDENTITY);
	comp->CreateUniform("uTileSizePixels",		UNIFORM_UINT,				1, &TILE_SIZE_PIXELS);
	comp->CreateUniform("uResX",				UNIFORM_UINT,				1, screenX);
	comp->CreateUniform("uResY",				UNIFORM_UINT,				1, screenY);
	comp->CreateUniform("uMaxLightsPerTile",	UNIFORM_UINT,				1, &MAX_NUM_LIGHTS_PER_TILE);
	comp->CreateUniform("uNumLights",			UNIFORM_UINT,				1, &UINT_FALSE);
	comp->CreateUniform("uDepth",				UNIFORM_TEXTUREBUFFER2D,	1, 0, depthMap);

	m_lightIndexBuffer->BindDataToGPU();
	m_lightIndexBuffer->DeleteLocalCopy();
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::InitializeFBO() {

	////////////////////////////////////////////////////
	//------------------------------------------
	//Sampler
	const Vector2* screenSize = BeirusRenderer::GetScreenSize();

	m_colorTarget = (IS_HDR_ENABLED) 
		? (new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_RGBA16F)) 
		: (new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_RGBA8));
	m_colorTarget->GenerateMipmap();

	m_normalsTarget		= new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_RGBA8);
	m_depthTarget		= new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_D24S8);


	////////////////////////////////////////////////////
	//------------------------------------------
	//Create the full screen mesh
	m_meshRenderer = new MeshRenderer();
	std::vector<uint16_t> meshIndices = { 0, 1, 2, 0, 2, 3 };
	s_fullScreenMesh	= BeirusMeshCollection::Get()->Allocate(&VertexDefinition::TEXTUREDVERTEX);
	s_scratchMesh2D		= BeirusMeshCollection::Get()->Allocate(&VertexDefinition::VERTEX2D_PCT);
	s_scratchLineMesh	= BeirusMeshCollection::Get()->Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);

	std::vector<TexturedVertex> m_vertsForMesh;

	m_vertsForMesh.push_back(TexturedVertex(Vector3(-1.f,  1.f, 0.f),	RGBA::WHITE, Vector2(0.f, 0.f))); //TL
	m_vertsForMesh.push_back(TexturedVertex(Vector3(-1.f, -1.f, 0.f),	RGBA::WHITE, Vector2(0.f, 1.f))); //BL
	m_vertsForMesh.push_back(TexturedVertex(Vector3( 1.f, -1.f, 0.f),	RGBA::WHITE, Vector2(1.f, 1.f))); //BR
	m_vertsForMesh.push_back(TexturedVertex(Vector3( 1.f,  1.f, 0.f),	RGBA::WHITE, Vector2(1.f, 0.f))); //TR

	Mesh* fsMesh = BeirusMeshCollection::Get()->GetMesh(s_fullScreenMesh);
	fsMesh->UpdateMesh(m_vertsForMesh, meshIndices);


	////////////////////////////////////////////////////
	//------------------------------------------
	//Create the FBO
	std::vector<TextureBuffer*> fboTextures1;
	fboTextures1.push_back(m_colorTarget);
	m_globalFBO = new Framebuffer(fboTextures1, m_depthTarget, IS_HDR_ENABLED);
}

//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::InitializeOpenGLDefinitions() {

	glGenBuffers				= (PFNGLGENBUFFERSPROC)				wglGetProcAddress("glGenBuffers");
	glBindBuffer				= (PFNGLBINDBUFFERPROC)				wglGetProcAddress("glBindBuffer");
	glBufferData				= (PFNGLBUFFERDATAPROC)				wglGetProcAddress("glBufferData");
	glGenerateMipmap			= (PFNGLGENERATEMIPMAPPROC)			wglGetProcAddress("glGenerateMipmap");
	glDeleteBuffers				= (PFNGLDELETEBUFFERSPROC)			wglGetProcAddress("glDeleteBuffers");
	glCreateShader				= (PFNGLCREATESHADERPROC)			wglGetProcAddress("glCreateShader");
	glShaderSource				= (PFNGLSHADERSOURCEPROC)			wglGetProcAddress("glShaderSource");
	glCompileShader				= (PFNGLCOMPILESHADERPROC)			wglGetProcAddress("glCompileShader");
	glGetShaderiv				= (PFNGLGETSHADERIVPROC)			wglGetProcAddress("glGetShaderiv");
	glDeleteShader				= (PFNGLDELETESHADERPROC)			wglGetProcAddress("glDeleteShader");
	glCreateProgram				= (PFNGLCREATEPROGRAMPROC)			wglGetProcAddress("glCreateProgram");
	glAttachShader				= (PFNGLATTACHSHADERPROC)			wglGetProcAddress("glAttachShader");
	glLinkProgram				= (PFNGLLINKPROGRAMPROC)			wglGetProcAddress("glLinkProgram");
	glGetProgramiv				= (PFNGLGETPROGRAMIVPROC)			wglGetProcAddress("glGetProgramiv");
	glDetachShader				= (PFNGLDETACHSHADERPROC)			wglGetProcAddress("glDetachShader");
	glDeleteProgram				= (PFNGLDELETEPROGRAMPROC)			wglGetProcAddress("glDeleteProgram");
	glGetShaderInfoLog			= (PFNGLGETSHADERINFOLOGPROC)		wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog			= (PFNGLGETPROGRAMINFOLOGPROC)		wglGetProcAddress("glGetProgramInfoLog");
	glGenVertexArrays			= (PFNGLGENVERTEXARRAYSPROC)		wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays		= (PFNGLDELETEVERTEXARRAYSPROC)		wglGetProcAddress("glDeleteVertexArrays");
	glBindVertexArray			= (PFNGLBINDVERTEXARRAYPROC)		wglGetProcAddress("glBindVertexArray");
	glGetAttribLocation			= (PFNGLGETATTRIBLOCATIONPROC)		wglGetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray	= (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer		= (PFNGLVERTEXATTRIBPOINTERPROC)	wglGetProcAddress("glVertexAttribPointer");
	glVertexAttribIPointer		= (PFNGLVERTEXATTRIBIPOINTERPROC)	wglGetProcAddress("glVertexAttribIPointer");
	glUseProgram				= (PFNGLUSEPROGRAMPROC)				wglGetProcAddress("glUseProgram");
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC)		wglGetProcAddress("glGetUniformLocation");
	glUniform1fv				= (PFNGLUNIFORM1FVPROC)				wglGetProcAddress("glUniform1fv");
	glUniform2fv				= (PFNGLUNIFORM2FVPROC)				wglGetProcAddress("glUniform2fv");
	glUniform3fv				= (PFNGLUNIFORM3FVPROC)				wglGetProcAddress("glUniform3fv");
	glUniform4fv				= (PFNGLUNIFORM4FVPROC)				wglGetProcAddress("glUniform4fv");
	glUniform1i					= (PFNGLUNIFORM1IPROC)				wglGetProcAddress("glUniform1i");
	glUniform1iv				= (PFNGLUNIFORM1IVPROC)				wglGetProcAddress("glUniform1iv");
	glUniform2iv				= (PFNGLUNIFORM2IVPROC)				wglGetProcAddress("glUniform2iv");
	glUniform3iv				= (PFNGLUNIFORM3IVPROC)				wglGetProcAddress("glUniform3iv");
	glUniform4iv				= (PFNGLUNIFORM4IVPROC)				wglGetProcAddress("glUniform4iv");
	glBindSampler				= (PFNGLBINDSAMPLERPROC)			wglGetProcAddress("glBindSampler");
	glGenSamplers				= (PFNGLGENSAMPLERSPROC)			wglGetProcAddress("glGenSamplers");
	glSamplerParameteri			= (PFNGLSAMPLERPARAMETERIPROC)		wglGetProcAddress("glSamplerParameteri");
	glDeleteSamplers			= (PFNGLDELETESAMPLERSPROC)			wglGetProcAddress("glDeleteSamplers");
	glUniformMatrix4fv			= (PFNGLUNIFORMMATRIX4FVPROC)		wglGetProcAddress("glUniformMatrix4fv");
	glActiveTexture				= (PFNGLACTIVETEXTUREPROC)			wglGetProcAddress("glActiveTexture");
	glGenFramebuffers			= (PFNGLGENFRAMEBUFFERSPROC)		wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer			= (PFNGLBINDFRAMEBUFFERPROC)		wglGetProcAddress("glBindFramebuffer");
	glFramebufferTexture		= (PFNGLFRAMEBUFFERTEXTUREPROC)		wglGetProcAddress("glFramebufferTexture");
	glCheckFramebufferStatus	= (PFNGLCHECKFRAMEBUFFERSTATUSPROC)	wglGetProcAddress("glCheckFramebufferStatus");
	glDrawBuffers				= (PFNGLDRAWBUFFERSPROC)			wglGetProcAddress("glDrawBuffers");
	glDeleteFramebuffers		= (PFNGLDELETEFRAMEBUFFERSPROC)		wglGetProcAddress("glDeleteFramebuffers");
	glBlitFramebuffer			= (PFNGLBLITFRAMEBUFFERPROC)		wglGetProcAddress("glBlitFramebuffer");
	glGetProgramResourceiv		= (PFNGLGETPROGRAMRESOURCEIVPROC)	wglGetProcAddress("glGetProgramResourceiv");
	glGetProgramResourceName	= (PFNGLGETPROGRAMRESOURCENAMEPROC)	wglGetProcAddress("glGetProgramResourceName");
	glGetActiveUniform			= (PFNGLGETACTIVEUNIFORMPROC)		wglGetProcAddress("glGetActiveUniform");
	glTexStorage2D				= (PFNGLTEXSTORAGE2DPROC)			wglGetProcAddress("glTexStorage2D");
	glTexImage2DMultisample		= (PFNGLTEXIMAGE2DMULTISAMPLEPROC)	wglGetProcAddress("glTexImage2DMultisample");
	glFramebufferTexture2D		= (PFNGLFRAMEBUFFERTEXTURE2DPROC)	wglGetProcAddress("glFramebufferTexture2D");
	glSamplerParameterf			= (PFNGLSAMPLERPARAMETERFPROC)		wglGetProcAddress("glSamplerParameterf");
	glDispatchCompute			= (PFNGLDISPATCHCOMPUTEPROC)		wglGetProcAddress("glDispatchCompute");
	glDispatchComputeIndirect	= (PFNGLDISPATCHCOMPUTEINDIRECTPROC)wglGetProcAddress("glDispatchComputeIndirect");
	glBindImageTexture			= (PFNGLBINDIMAGETEXTUREPROC)		wglGetProcAddress("glBindImageTexture");
	glMemoryBarrier				= (PFNGLMEMORYBARRIERPROC)			wglGetProcAddress("glMemoryBarrier");
	glMapBuffer					= (PFNGLMAPBUFFERPROC)				wglGetProcAddress("glMapBuffer");
	glUnmapBuffer				= (PFNGLUNMAPBUFFERPROC)			wglGetProcAddress("glUnmapBuffer");
	glBindBufferBase			= (PFNGLBINDBUFFERBASEPROC)			wglGetProcAddress("glBindBufferBase");
	glGetInteger64v				= (PFNGLGETINTEGER64VPROC)			wglGetProcAddress("glGetInteger64v");
	glGenQueries				= (PFNGLGENQUERIESPROC)				wglGetProcAddress("glGenQueries");
	glQueryCounter				= (PFNGLQUERYCOUNTERPROC)			wglGetProcAddress("glQueryCounter");
	glGetQueryObjectiv			= (PFNGLGETQUERYOBJECTIVPROC)		wglGetProcAddress("glGetQueryObjectiv");
	glGetQueryObjectui64v		= (PFNGLGETQUERYOBJECTUI64VPROC)	wglGetProcAddress("glGetQueryObjectui64v");
	glUniform1ui				= (PFNGLUNIFORM1UIPROC)				wglGetProcAddress("glUniform1ui");
	glUniform1uiv				= (PFNGLUNIFORM1UIVPROC)			wglGetProcAddress("glUniform1uiv");
	glUniform2uiv				= (PFNGLUNIFORM2UIVPROC)			wglGetProcAddress("glUniform2uiv");
	glUniform3uiv				= (PFNGLUNIFORM3UIVPROC)			wglGetProcAddress("glUniform3uiv");
	glUniform4uiv				= (PFNGLUNIFORM4UIVPROC)			wglGetProcAddress("glUniform4uiv");
	glBufferSubData				= (PFNGLBUFFERSUBDATAPROC)			wglGetProcAddress("glBufferSubData");
	glGetDebugMessageLog		= (PFNGLGETDEBUGMESSAGELOGPROC)		wglGetProcAddress("glGetDebugMessageLog");
	glDebugMessageControl		= (PFNGLDEBUGMESSAGECONTROLPROC)	wglGetProcAddress("glDebugMessageControl");
	glDebugMessageCallback		= (PFNGLDEBUGMESSAGECALLBACKPROC)	wglGetProcAddress("glDebugMessageCallback");
	glEnablei					= (PFNGLENABLEIPROC)				wglGetProcAddress("glEnablei");
	glDisablei					= (PFNGLDISABLEIPROC)				wglGetProcAddress("glDisablei");
	glBlendFunci				= (PFNGLBLENDFUNCIPROC)				wglGetProcAddress("glBlendFunci");
	glClearBufferfv				= (PFNGLCLEARBUFFERFVPROC)			wglGetProcAddress("glClearBufferfv");
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::Update(float deltaSeconds) {
	UNUSED(deltaSeconds);

	PlayerCamera3D::Update(deltaSeconds);

	ShaderProgram* fxaaProg = ShaderProgram::CreateOrGetShaderProgram("FXAA");
	Uniform* gUsingFXAA = fxaaProg->GetUniform("gUsingFXAA");
	gUsingFXAA->SetData((void*)&m_usingFXAA);

	Tonemapping::Get()->Update(deltaSeconds);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FORWARD+
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::RunLightCullingComputeShader() {

	ComputeShaderProgram* comp = ComputeShaderProgram::CreateOrGetComputeShaderProgram("LightCulling");

	//UPDATE UNIFORMS
	Uniform* uProj		= comp->GetUniform("uProj");
	Uniform* uView		= comp->GetUniform("uView");
	Uniform* uNumLights = comp->GetUniform("uNumLights");

	uProj				->SetData((void*)GetProjectionMatrix());
	uNumLights			->SetData((void*)LightSystem::GetNumLightsInScene());
	uView				->SetData((void*)PlayerCamera3D::GetViewMatrix());

	//m_lightIndexBuffer	->BindDataToGPU();
	
	comp->Dispatch(GetNumTilesX(), GetNumTilesY(), 1);
	GL_CHECK(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::InternalRenderScene(Scene* sceneToRender) {

	LightSystem::UpdateLightsOnMultilightShader();
	LightSystem::UpdateShadowCastingLights(sceneToRender);

	DynamicReflectionProbeManager::Get()->UpdateProbes(sceneToRender);

	Framebuffer::BindBackBuffer();
	DisableBlending();
	EnableDepthWriting();
	EnableDepthTesting();
	EnableColorWriting();
	ClearScreen(RGBA(0.1f, 0.1f, 0.1f, 1.f));

	m_globalFBO->Bind();

	ClearScreen(RGBA(0.1f, 0.1f, 0.1f, 1.f));

	sceneToRender->RenderSkybox();

	ClearDepth();

	DepthAndNormalsPass::Get()->RunPass(sceneToRender);

	RunLightCullingComputeShader();

	SSAO::RenderSSAOMap();

	m_globalFBO->Bind();

	//m_gpuTimer->Start();
	Render3DElements(sceneToRender);

	OIT::Get()->RenderTransparentElementsForBlending(sceneToRender);	
	RenderLines();

	EnableBlending();
	BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//m_gpuTimer->Start();
	Tonemapping::Get()->RunPass();
	//m_gpuTimer->Stop();
	//DebuggerPrintf("Tonemapping: %.4f\n", m_gpuTimer->GetDifference());

	Framebuffer::BindBackBuffer();

	//m_gpuTimer->Start();
	BeirusRenderer::RenderFXAA();
	//m_gpuTimer->Stop();
	//DebuggerPrintf("FXAA: %.4f\n", m_gpuTimer->GetDifference());

	DynamicReflectionProbeManager::Get()->RenderOutlines();


	//m_gpuTimer->Start();
	Render2DElements(); 
	//m_gpuTimer->Stop();
	//DebuggerPrintf("Render 2D: %.4f\n\n", m_gpuTimer->GetDifference());
	DisableBlending();
}

//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::RenderDepthAndNormals(Scene* sceneToRender) {

	EnableDepthWriting();
	SetDepthTestingMode(GL_LESS);

	Material* preDepthPassMat = MaterialBank::GetMaterial("DepthAndNormalsPass");
	sceneToRender->RenderWithMaterial(preDepthPassMat);
}


//---------------------------------------------------------------------------------------------------------------------------+
void BeirusRenderer::Render3DElements(Scene* sceneToRender) {

	EnableDepthTesting();
	EnableBackfaceCulling();
	EnableColorWriting();
	DisableDepthWriting();
	SetDepthTestingMode(GL_EQUAL);

	sceneToRender->Render();
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::RenderLines() {

	DisableBlending();

	LineWidth(4.f);
	EnableDepthTesting();

	for (size_t i = 0; i < m_linesToRender.size(); i++) {
		Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(s_scratchLineMesh);
		mesh->UpdateMesh(m_linesToRender[i].m_verts, m_linesToRender[i].m_meshIndices);
		m_meshRenderer->RenderMeshWithMaterial(s_scratchLineMesh, m_linesToRender[i].m_material, Matrix4::IDENTITY, true);
	}

	LineWidth(1.f);
	DisableDepthTesting();

	for (size_t i = 0; i < m_linesToRender.size(); i++) {
		Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(s_scratchLineMesh);
		mesh->UpdateMesh(m_linesToRender[i].m_verts, m_linesToRender[i].m_meshIndices);
		m_meshRenderer->RenderMeshWithMaterial(s_scratchLineMesh, m_linesToRender[i].m_material, Matrix4::IDENTITY, true);
	}

	m_linesToRender.clear();
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::RenderTransparentElements(Scene* sceneToRender) {

	sceneToRender->Render(true);
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::Render2DElements() {

	//EnableBlending();
	//BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DisableDepthTesting();

	MeshRenderer meshRenderer;
	
	for (size_t i = 0; i < m_rQuadsToRender.size(); i++) {
		Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(s_scratchMesh2D);
		mesh->UpdateMesh(m_rQuadsToRender[i].m_verts, m_rQuadsToRender[i].m_meshIndices);
		meshRenderer.RenderMeshWithMaterial(s_scratchMesh2D, m_rQuadsToRender[i].m_material, Matrix4::IDENTITY);
	}
	
	m_rQuadsToRender.clear();

	//DisableBlending();
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::RenderFullScreenQuad() {

	DisableBackfaceCulling();
	m_meshRenderer->RenderMeshWithMaterial(s_fullScreenMesh, m_defaultFBOMaterial, Matrix4::IDENTITY);
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusRenderer::RenderFXAA() {

	EnableBlending();
	DisableDepthTesting();
	DisableDepthWriting();
	DisableBackfaceCulling();
	//GL_CHECK(glDisable(GL_FRAMEBUFFER_SRGB));

	m_meshRenderer->RenderMeshWithMaterial(s_fullScreenMesh, m_fxaaMat, Matrix4::IDENTITY);

	DisableDepthTesting();
	DisableBackfaceCulling();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusRenderer::AttachPlayerCamera3D(PlayerCamera3D* pCam3D) {

	s_theBeirusRenderer->m_playerCamera = pCam3D;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OUTSIDE OF CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC double BeirusRenderer::GetGPUTime() {
	return s_theBeirusRenderer->m_gpuTimer->GetDifference();
}