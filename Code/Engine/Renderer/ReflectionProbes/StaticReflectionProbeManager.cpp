#include "Engine/Renderer/ReflectionProbes/StaticReflectionProbeManager.hpp"
#include "Engine/Renderer/ReflectionProbes/StaticReflectionProbe.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Scene/Scene.hpp"

STATIC StaticReflectionProbeManager* StaticReflectionProbeManager::s_pManager = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GET
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC StaticReflectionProbeManager* StaticReflectionProbeManager::Get() {

	if (s_pManager == nullptr) {
		s_pManager = new StaticReflectionProbeManager();
		BeirusEngine::RegisterShutdownCallback(&StaticReflectionProbeManager::Shutdown);
	}

	return s_pManager;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOAD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
size_t StaticReflectionProbeManager::LoadProbesInDirectory(const String& dir, Scene* sceneToLoadInTo) {

	std::vector<String> probeFiles = FileUtils::EnumerateFiles(dir, "*.xml", true);

	for (size_t i = 0; i < probeFiles.size(); i++) {

		StaticReflectionProbe* newProbe = StaticReflectionProbe::Load(probeFiles[i], sceneToLoadInTo);
		m_sProbes.push_back(newProbe);
	}

	m_numProbes = m_sProbes.size();
	return m_numProbes;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void StaticReflectionProbeManager::BindProbesToShader(const String& shaderName) {

	ShaderProgram* prog = ShaderProgram::CreateOrGetShaderProgram(shaderName);

	Uniform* count = prog->GetUniform("envMapCount");
	count->SetData((void*)&m_numProbes);

	for (size_t i = 0; i < m_sProbes.size(); i++) {

		StaticReflectionProbe* currProbe = m_sProbes[i];

		//BIND MAP
		String uniName = StringUtils::Stringf("gTexEnv%u", i);
		Uniform* envMapUni = prog->GetUniform(uniName.c_str());
		envMapUni->SetData(currProbe->GetEnvironmentMap());

		//BIND POS
		uniName = StringUtils::Stringf("envPos%u", i);
		Uniform* radiusUni = prog->GetUniform(uniName.c_str());
		radiusUni->SetData(&currProbe->m_position);

		//BIND MINS
		uniName = StringUtils::Stringf("envMins%u", i);
		radiusUni = prog->GetUniform(uniName.c_str());
		radiusUni->SetData(&currProbe->m_boxMins);

		//BIND MAXS
		uniName = StringUtils::Stringf("envMaxs%u", i);
		Uniform* posUni = prog->GetUniform(uniName.c_str());
		posUni->SetData(&currProbe->m_boxMaxs);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
StaticReflectionProbeManager::StaticReflectionProbeManager() {

}


//---------------------------------------------------------------------------------------------------------------------------
StaticReflectionProbeManager::~StaticReflectionProbeManager() {

	for (size_t i = 0; i < m_sProbes.size(); i++) {
		delete m_sProbes[i];
		m_sProbes[i] = nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void StaticReflectionProbeManager::Shutdown() {

	delete s_pManager;
	s_pManager = nullptr;
}