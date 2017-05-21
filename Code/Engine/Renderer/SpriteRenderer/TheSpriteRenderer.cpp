#include "Engine/Renderer/SpriteRenderer/TheSpriteRenderer.hpp"
#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteDatabase.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Profiler/Profiler.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"

STATIC TheSpriteRenderer*	TheSpriteRenderer::s_theSpriteRenderer	= nullptr;
STATIC MeshID				TheSpriteRenderer::s_mesh				= 0;
STATIC MeshID				TheSpriteRenderer::s_fullScreenMesh		= 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATIC INTERFACE
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC void TheSpriteRenderer::Initialize(float virtualSize,float aspectRatio) {

	s_theSpriteRenderer = new TheSpriteRenderer(virtualSize, aspectRatio);
	s_mesh				= BeirusMeshCollection::Get()->Allocate(&VertexDefinition::VERTEX2D_PCT);
	Mesh* newMesh		= BeirusMeshCollection::Get()->GetMesh(s_mesh);

	std::vector<uint16_t> meshIndices = {
		0, 1, 2,
		0, 2, 3
	};

	newMesh->UpdateMesh(meshIndices);


	//INIT FULLSCREEN MESH
	s_fullScreenMesh = BeirusMeshCollection::Get()->Allocate(&VertexDefinition::TEXTUREDVERTEX);
	Mesh* fsMesh = BeirusMeshCollection::Get()->GetMesh(s_fullScreenMesh);

	std::vector<TexturedVertex> m_vertsForMesh;
	m_vertsForMesh.push_back(TexturedVertex(Vector3(-1.f, 1.f, 0.f), RGBA::WHITE, Vector2(0.f, 0.f))); //TL
	m_vertsForMesh.push_back(TexturedVertex(Vector3(-1.f, -1.f, 0.f), RGBA::WHITE, Vector2(0.f, 1.f))); //BL
	m_vertsForMesh.push_back(TexturedVertex(Vector3(1.f, -1.f, 0.f), RGBA::WHITE, Vector2(1.f, 1.f))); //BR
	m_vertsForMesh.push_back(TexturedVertex(Vector3(1.f, 1.f, 0.f), RGBA::WHITE, Vector2(1.f, 0.f))); //TR

	fsMesh->UpdateMesh(m_vertsForMesh, meshIndices);

	RegisterSpriteRendererCommands();

	BeirusEngine::RegisterUpdateCallback(MakeDelegate(s_theSpriteRenderer, &TheSpriteRenderer::InternalUpdate));
}
STATIC void TheSpriteRenderer::RegisterSpriteRendererCommands() {
	Console::RegisterCommand("layer", "\"layer <enable,disable> layer_name\", Enables or disables a sprite layer by name", LayerToggle);
}
STATIC void TheSpriteRenderer::Destroy() {
	delete s_theSpriteRenderer;
	SpriteLayer::DestroySpriteLayers();
	SpriteResource::DestroySpriteResources();
	SpriteDatabase::Destroy();
	ParticleSystemDefinition2D::DeinitializeAllDefinitions();
	ParticleEmitterDefinition2D::DeinitializeAllDefinitions();
}

STATIC void TheSpriteRenderer::RegisterSprite(Sprite* sprite) {
	SpriteLayer* layer = SpriteLayer::CreateOrGetLayer(sprite->GetLayer().c_str());
	layer->AddToList(sprite);
}
STATIC void TheSpriteRenderer::UnregisterSprite(Sprite* sprite) {
	SpriteLayer* layer = SpriteLayer::CreateOrGetLayer(sprite->GetLayer().c_str());
	if(layer)
		layer->RemoveFromList(sprite);
}
STATIC void TheSpriteRenderer::CreateLayer(const String& layer) {
	s_theSpriteRenderer->m_layers.push_back(SpriteLayer::CreateOrGetLayer(layer));
}

//---------------------------------------------------------------------------------------------------------------------------
//CAMERA
//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheSpriteRenderer::UpdateCamera(const Camera2D& cam) {
	s_theSpriteRenderer->UpdateViewMatrix(cam);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//INTERNAL
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS INIT
//---------------------------------------------------------------------------------------------------------------------------
TheSpriteRenderer::TheSpriteRenderer(float virtualSize, float aspectRatio) 
	: m_defaultMaterial(nullptr)
	, m_virtualSize(virtualSize)
	, m_aspectRatio(aspectRatio)
	, m_importSize(25.f)
	, m_meshRenderer(MeshRenderer())
{
	m_virtualSize = virtualSize;
	m_aspectRatio = aspectRatio;

	if (m_aspectRatio < 1.f) {
		m_virtualBounds.y = m_virtualSize / aspectRatio;
		m_virtualBounds.x = m_virtualSize;
	}
	else {
		m_virtualBounds.x = m_virtualSize * aspectRatio;
		m_virtualBounds.y = m_virtualSize;
	}

	m_virtualRect = AABB2(Vector2(-m_virtualBounds.x / 2.f, -m_virtualBounds.y / 2.f), Vector2(m_virtualBounds.x / 2.f, m_virtualBounds.y / 2.f));

	m_ortho.ChangeToProjectionMatrix(m_virtualBounds.x, m_virtualBounds.y, -1.f, 1.f);
	InitializeDefaultShaders();

	const Vector2* screenSize = BeirusRenderer::GetScreenSize();

	m_currentColorTarget = new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_RGBA8);
	std::vector<TextureBuffer*> fboTextures1;
	fboTextures1.push_back(m_currentColorTarget);
	m_currentSurface = new Framebuffer(fboTextures1, nullptr);

	m_effectColorTarget = new TextureBuffer((int)screenSize->x, (int)screenSize->y, TEXTURE_FORMAT_D24S8);
	std::vector<TextureBuffer*> fboTextures2;
	fboTextures2.push_back(m_effectColorTarget);
	m_effectSurface = new Framebuffer(fboTextures2, nullptr);

	BeirusRenderer::DisableBackfaceCulling();
	BeirusRenderer::DisableDepthTesting();

	m_view = Matrix4::IDENTITY;
}
TheSpriteRenderer::~TheSpriteRenderer() { 
	delete m_currentSurface;
	delete m_effectSurface;
	delete m_defaultMaterial;
	delete m_currentColorTarget;
	delete m_effectColorTarget;
}

void TheSpriteRenderer::InitializeDefaultShaders() {
	ShaderProgram* defaultProg = ShaderProgram::CreateOrGetShaderProgram("DefaultSprite");

	Texture* defaultMatTex = Texture::CreateOrGetTexture("Data/Textures/DefaultMaterial.png");

	defaultProg->CreateUniform("gTexDiffuse",	UNIFORM_TEXTURE2D,	1, defaultMatTex);
	defaultProg->CreateUniform("gView",			UNIFORM_MAT4,		1, &m_view);
	defaultProg->CreateUniform("gProj",			UNIFORM_MAT4,		1, &m_ortho);

	m_defaultMaterial = new Material("DefaultSprite");
}

void TheSpriteRenderer::InternalSetImportSize(float importSize) {
	m_importSize = importSize;
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void TheSpriteRenderer::InternalUpdate(float deltaSeconds) {
	
	for (uint i = 0; i < m_layers.size(); i++) {

		if (!m_layers[i]->IsEnabled())
			continue;

		m_layers[i]->Update(deltaSeconds);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheSpriteRenderer::UpdateViewAndRender(const Camera2D& cam) {
	UpdateViewMatrix(cam);
	InternalRender();
}
void TheSpriteRenderer::InternalRender() {


	Framebuffer::BindBackBuffer();
	BeirusRenderer::ClearScreen(RGBA(0.1f, 0.1f, 0.1f, 1.f));

	m_currentSurface->Bind();
	BeirusRenderer::ClearScreen(RGBA(0.1f, 0.1f, 0.1f, 1.f));

	for (uint i = 0; i < m_layers.size(); i++) {

		if (!m_layers[i]->IsEnabled())
			continue;

		RenderLayer(m_layers[i]);

		//WARNING: COPYTEXTUREBUFFERTOMATERIAL NO LONGER VALID
		//m_layers[i]->m_effect->CopyTextureBufferToMaterial(m_currentSurface->GetColorTexture());

		m_effectSurface->Bind();
		RenderFullScreenQuad(m_layers[i]->m_effect);


		std::swap(m_currentSurface, m_effectSurface);
	}

	m_currentSurface->CopyToBack();
}
void TheSpriteRenderer::RenderLayer(SpriteLayer* currLayer) {

	m_currentSurface->Bind();

	Sprite* curr = currLayer->GetHead();

	for (uint j = 0; j < currLayer->GetNumSpritesInLayer(); j++) {

		if (!Cull(curr))
			DrawSprite(curr);
		curr = curr->m_next;
	}
}
void TheSpriteRenderer::DrawSprite(Sprite* sprite) {

	Profiler::PushFrame("DrawSprite");

	CopyToMesh(sprite);

	UpdateViewInMaterial(sprite->GetMaterial());

	m_meshRenderer.RenderMeshWithMaterial(s_mesh, sprite->GetMaterial(), Matrix4::IDENTITY);

	Profiler::PopFrame();
}

STATIC void TheSpriteRenderer::CopyToMesh(Sprite* sprite) {

	std::vector<Vertex2D_PCT> vertsForMesh;
	Vector2 dim = sprite->GetSpriteResource()->m_dimensions;

	Vector2 v0 = Vector2(-sprite->m_pivot.x, -sprite->m_pivot.y);
	Vector2 v1 = Vector2(dim.x - sprite->m_pivot.x, -sprite->m_pivot.y);
	Vector2 v2 = Vector2(dim.x - sprite->m_pivot.x, dim.y - sprite->m_pivot.y);
	Vector2 v3 = Vector2(-sprite->m_pivot.x, dim.y - sprite->m_pivot.y);

	float Sx = sprite->m_scale.x;
	float Sy = sprite->m_scale.y;
	float sin = SinDegrees(sprite->m_rotation);
	float cos = CosDegrees(sprite->m_rotation);

	Vector2 v0Prime = Vector2(v0.x * Sx * cos + v0.y * Sy * sin, -v0.x * Sx * sin + v0.y * Sy * cos) + sprite->m_position;
	Vector2 v1Prime = Vector2(v1.x * Sx * cos + v1.y * Sy * sin, -v1.x * Sx * sin + v1.y * Sy * cos) + sprite->m_position;
	Vector2 v2Prime = Vector2(v2.x * Sx * cos + v2.y * Sy * sin, -v2.x * Sx * sin + v2.y * Sy * cos) + sprite->m_position;
	Vector2 v3Prime = Vector2(v3.x * Sx * cos + v3.y * Sy * sin, -v3.x * Sx * sin + v3.y * Sy * cos) + sprite->m_position;

	Vector2 bl = sprite->GetSpriteResource()->m_texCoordsRect.mins;
	Vector2 tr = sprite->GetSpriteResource()->m_texCoordsRect.maxs;
	Vector2 v0TexCoord = Vector2(bl.x, bl.y);
	Vector2 v1TexCoord = Vector2(tr.x, bl.y);
	Vector2 v2TexCoord = Vector2(tr.x, tr.y);
	Vector2 v3TexCoord = Vector2(bl.x, tr.y);


	vertsForMesh.push_back(Vertex2D_PCT(v0Prime, sprite->m_tint, v0TexCoord)); //TL
	vertsForMesh.push_back(Vertex2D_PCT(v1Prime, sprite->m_tint, v1TexCoord)); //BL
	vertsForMesh.push_back(Vertex2D_PCT(v2Prime, sprite->m_tint, v2TexCoord)); //BR
	vertsForMesh.push_back(Vertex2D_PCT(v3Prime, sprite->m_tint, v3TexCoord)); //TR

	Mesh* sMesh = BeirusMeshCollection::Get()->GetMesh(s_mesh);
	sMesh->UpdateMesh(vertsForMesh);
}

bool TheSpriteRenderer::Cull(Sprite* s) const {
	UNREFERENCED_PARAMETER(s);
/*
	Vector2 dim = s->GetSpriteResource()->m_dimensions;

	float radius = dim.Length() / 2.f;
	Vector2 posToCheck = s->m_position;

	if (IsDiskInSquare(posToCheck, radius, m_virtualRect)) {
		return false;
	}
	else {
		return false;
	}*/

	return false;
	//return IsDiskInSquare(posToCheck, radius, m_virtualRect);
}

//---------------------------------------------------------------------------------------------------------------------------
//LAYER EFFECTS
//---------------------------------------------------------------------------------------------------------------------------
void TheSpriteRenderer::InternalAddLayerEffect(const String& layerName, const String& shaderProgName) {

	SpriteLayer* layerToAddEffectTo = SpriteLayer::GetLayer(layerName);
	ASSERT_OR_DIE(layerToAddEffectTo, "ERROR: Could not add layer effect to non-existing layer");

	layerToAddEffectTo->AddEffect(shaderProgName);
}

STATIC void TheSpriteRenderer::EnableLayer(const String& layerName) {
	SpriteLayer* layer = s_theSpriteRenderer->GetLayer(layerName);

	if (!layer) {
		String output = StringUtils::Stringf("Could not find layer with name %s", layerName.c_str());
		Console::PrintOutput(output, CONSOLE_WARNING);
	}
	else {
		layer->Enable();
	}
}
STATIC void TheSpriteRenderer::DisableLayer(const String& layerName) {
	SpriteLayer* layer = s_theSpriteRenderer->GetLayer(layerName);

	if (!layer) {
		String output = StringUtils::Stringf("Could not find layer with name %s", layerName.c_str());
		Console::PrintOutput(output, CONSOLE_WARNING);
	}
	else {
		layer->Disable();
	}
}

void TheSpriteRenderer::RenderFullScreenQuad(Material* mat) {
	m_meshRenderer.RenderMeshWithMaterial(s_fullScreenMesh, mat, Matrix4::IDENTITY);
}

SpriteLayer* TheSpriteRenderer::GetLayer(const String& name) {
	SpriteLayer* layer = nullptr;

	for (uint i = 0; i < m_layers.size(); i++) {
		if (m_layers[i]->GetName() == name) {
			layer = m_layers[i];
		}
	}

	return layer;
}

//---------------------------------------------------------------------------------------------------------------------------
//CAMERA
//---------------------------------------------------------------------------------------------------------------------------
void TheSpriteRenderer::UpdateViewMatrix(const Camera2D& cam) {
	m_view = cam.GetViewMatrix();
}
void TheSpriteRenderer::UpdateViewInMaterial(Material* mat) const {

	Uniform* view = mat->GetUniform("gView");
	view->SetData((void*)&m_view);
}

//---------------------------------------------------------------------------------------------------------------------------
//CONSOLE COMMANDS
//---------------------------------------------------------------------------------------------------------------------------
void LayerToggle(Command& args) {
	String toggle, layerName;

	args.GetNextString(toggle);
	args.GetNextString(layerName);

	if (toggle == "enable") {
		TheSpriteRenderer::EnableLayer(layerName);
		String output = StringUtils::Stringf("Layer %s enabled.", layerName.c_str());
		Console::PrintOutput(output, CONSOLE_WARNING);
	}
	else if (toggle == "disable") {
		TheSpriteRenderer::DisableLayer(layerName);
		String output = StringUtils::Stringf("Layer %s disabled.", layerName.c_str());
		Console::PrintOutput(output, CONSOLE_WARNING);
	}
	else {
		Console::PrintOutput("Unknown command.", CONSOLE_WARNING);
	}
	
}