#include "Engine/Renderer/Effects/Tonemapping.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"


STATIC Tonemapping* Tonemapping::s_HDR = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC Tonemapping* Tonemapping::Get() {

	if (s_HDR == nullptr) {
		s_HDR = new Tonemapping();
		BeirusEngine::RegisterShutdownCallback(&Tonemapping::Shutdown);
	}

	return s_HDR;
}


//---------------------------------------------------------------------------------------------------------------------------
void Tonemapping::Update(float deltaSeconds) {

	UNUSED(deltaSeconds);

	m_targetExposure = m_defaultExposure;
	Vector3 cameraPos = BeirusRenderer::GetPlayerCamera()->m_position;

	if (m_exposureVolumesEnabled) {
		for (size_t idx = 0; idx < m_exposureVolumes.size(); idx++) {
			if (IsPointInBox(m_exposureVolumes[idx].m_volume, cameraPos)) {
				m_targetExposure = m_exposureVolumes[idx].m_exposureVal;
				break;
			}
		}
	}

	//if (InputSystem::GetKeyDown('B')) {
	//	m_debugDraw = !m_debugDraw;
	//}
	//if (InputSystem::GetKeyDown('N')) {
	//	if (m_targetExposure == 6.f) {
	//		m_targetExposure = 1.6f;
	//	}
	//	else if (m_targetExposure == 1.6f) {
	//		m_targetExposure = 6.f;
	//	}
	//}

	m_exposure = m_exposure + (m_targetExposure - m_exposure) * (.01f);
}


//---------------------------------------------------------------------------------------------------------------------------
void Tonemapping::RunPass() {

	//RUN TONEMAPPING
	m_tonemapFBO->Bind();
	BeirusRenderer::DisableBlending();
	BeirusRenderer::DisableBackfaceCulling();
	MeshRenderer* mr	= BeirusRenderer::GetGlobalMeshRenderer();
	mr->RenderMeshWithMaterial(m_fullScreenMesh, m_tonemappingMat, Matrix4::IDENTITY);
	BeirusRenderer::EnableBlending();
	BeirusRenderer::BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_debugVisualizer) {
		for (size_t i = 0; i < m_exposureVolumes.size(); i++) {
			BeirusRenderer::DrawBoxLineMesh(m_exposureVolumes[i].m_volume, RGBA::WHITE);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Tonemapping::Tonemapping() {

	m_colorTarget		= BeirusRenderer::GetDefaultColorTarget();

	m_tonemappingMat	= MaterialBank::GetMaterial("Tonemapping");
	m_tonemappingMat->m_renderState.m_backfaceCulling = false;

	m_tonemappingMat->CreateUniform("gExposure",			UNIFORM_FLOAT,				1,		&m_exposure);
	//m_tonemappingMat->CreateUniform("gDebugDraw",			UNIFORM_BOOL,				1,		&m_debugDraw);

	std::vector<TextureBuffer*> tonemapTargets;
	tonemapTargets.push_back(m_colorTarget);
	m_tonemapFBO = new Framebuffer(tonemapTargets, nullptr);

	m_fullScreenMesh = BeirusRenderer::GetGlobalFullscreenMesh();
}


//---------------------------------------------------------------------------------------------------------------------------
Tonemapping::~Tonemapping() {
	//SAFE_DELETE(m_exposureTarget);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Tonemapping::Shutdown() {
	SAFE_DELETE(s_HDR);
}