#include "Engine/Renderer/ReflectionProbes/DynamicReflectionProbeManager.hpp"
#include "Engine/Renderer/ReflectionProbes/DynamicReflectionProbe.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Scene/Scene.hpp"


DynamicReflectionProbeManager* DynamicReflectionProbeManager::s_DRPManager = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GET
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC DynamicReflectionProbeManager* DynamicReflectionProbeManager::Get() {

	if (s_DRPManager == nullptr) {
		s_DRPManager = new DynamicReflectionProbeManager();
		BeirusEngine::RegisterShutdownCallback(&DynamicReflectionProbeManager::Destroy);
	}

	return s_DRPManager;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbeManager::UpdateProbes(Scene* sceneToRender) {

	BeirusRenderer::EnableColorWriting();
	BeirusRenderer::EnableDepthWriting();
	BeirusRenderer::EnableDepthTesting();

	//for (uint8_t idx = 0; idx < m_probes.size(); idx++) {
	//	m_probes[idx]->Update(sceneToRender);
	//}

	if (m_probes.empty()) {
		return;
	}

	m_probes[m_currRendering]->Update(sceneToRender);

	m_currRendering++;
	if (m_currRendering == m_probes.size()) {
		m_currRendering = 0;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbeManager::RenderOutlines() {

	for (uint8_t idx = 0; idx < m_probes.size(); idx++) {
		m_probes[idx]->Render();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
DynamicReflectionProbeManager::DynamicReflectionProbeManager() {


}


//---------------------------------------------------------------------------------------------------------------------------
DynamicReflectionProbeManager::~DynamicReflectionProbeManager() {

	for (uint8_t i = 0; i < m_probes.size(); i++) {
		delete m_probes[i];
		m_probes[i] = nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void DynamicReflectionProbeManager::Destroy() {

	delete s_DRPManager;
	s_DRPManager = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbeManager::AttachReflectionProbe(DynamicReflectionProbe* newProbe) {

	ASSERT_OR_DIE(m_probes.size() != MAX_NUM_DYNAMIC_REFLECTION_PROBES, "ERROR: Too many dynamic reflection probes in scene.");
	m_probes.push_back(newProbe);
}


//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbeManager::DetachReflectionProbe(DynamicReflectionProbe* probe) {

	DynamicReflectionProbe* probeToRemove = nullptr;
	uint8_t idx = 0;

	for (idx = 0; idx < m_probes.size(); idx++) {
		if (probe == m_probes[idx]) {
			probeToRemove = m_probes[idx];
		}
	}

	ASSERT_OR_DIE(probeToRemove != nullptr, "ERROR: Invalid probe to remove. This shouldn't have happened.");
	delete probeToRemove;
	probeToRemove = nullptr;

	m_probes.erase(m_probes.begin() + idx);
}