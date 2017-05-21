#include "Engine/Particles/ParticleSystemManager.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Utils/GeneralUtils.hpp"
#include "Engine/Particles/ParticleDefinition.hpp"
#include "Engine/Particles/ParticleSystemDefinition.hpp"
#include "Engine/Particles/ParticleEmitterDefinition.hpp"
#include "Engine/Particles/ParticleSystem.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

STATIC ParticleSystemManager* ParticleSystemManager::s_particleSystemManager = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOAD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystemManager::LoadFile(const char* filename) {

	ParticleSystemManager* pManager = GetInstance();

	XMLNode root = XMLNode::parseFile(filename);
	root = root.getChildNode(0);

	for (int defIdx = 0; defIdx < root.nChildNode(); defIdx++) {

		XMLNode defNode = root.getChildNode(defIdx);

		if (strcmp(defNode.getName(), PARTICLE_DEFINITION) == 0) {
			ParticleDefinition* newPDef = ParticleDefinition::LoadFromXML(defNode);
			size_t nameHash = HashCString(newPDef->GetName());
			pManager->m_particleDefs.insert(ParticleDefPair(nameHash, newPDef));
		}
		else if (strcmp(defNode.getName(), PARTICLE_EMITTER_DEFINITION) == 0) {
			ParticleEmitterDefinition* newEmitterDef = ParticleEmitterDefinition::LoadFromXML(defNode);
			size_t nameHash = HashCString(newEmitterDef->GetName());
			pManager->m_emitterDefs.insert(ParticleEmitterDefMapPair(nameHash, newEmitterDef));
		}
		else if (strcmp(defNode.getName(), PARTICLE_SYSTEM_DEFINITION) == 0) {
			ParticleSystemDefinition* newSystemDef = ParticleSystemDefinition::LoadFromXML(defNode);
			size_t nameHash = HashCString(newSystemDef->GetName());
			pManager->m_systemDefs.insert(ParticleSystemDefPair(nameHash, newSystemDef));
		}
	}

	//TODO: Check to make sure if valid .xml file
	return true;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystemManager::LoadFiles(const char* dir) {

	std::vector<String> filenames = FileUtils::EnumerateFiles(dir, "*.xml", true);

	for (uint i = 0; i < filenames.size(); i++) {
		LoadFile(filenames[i].c_str());
	}

	return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystemManager::AddEmitterDefinition(ParticleEmitterDefinition* emitterDef) {

	size_t nameHash = HashCString(emitterDef->GetName());

	ParticleEmitterDefMapIterator emitterIt = GetInstance()->m_emitterDefs.find(nameHash);
	if (emitterIt != GetInstance()->m_emitterDefs.end()) {
		GetInstance()->m_emitterDefs.insert(ParticleEmitterDefMapPair(nameHash, emitterDef));
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystemManager::AddSystemDefinition(ParticleSystemDefinition* systemDef) {

	size_t nameHash = HashCString(systemDef->GetName());

	ParticleSystemDefIterator systemIt = GetInstance()->m_systemDefs.find(nameHash);
	if (systemIt != GetInstance()->m_systemDefs.end()) {
		GetInstance()->m_systemDefs.insert(ParticleSystemDefPair(nameHash, systemDef));
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystemManager::AddParticleDefinition(ParticleDefinition* particleDef) {

	size_t nameHash = HashCString(particleDef->GetName());

	ParticleDefIterator systemIt = GetInstance()->m_particleDefs.find(nameHash);
	if (systemIt != GetInstance()->m_particleDefs.end()) {
		GetInstance()->m_particleDefs.insert(ParticleDefPair(nameHash, particleDef));
		return true;
	}
	else {
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GET
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleDefinition* ParticleSystemManager::GetParticleDefinition(const char* name) {

	size_t nameHash = HashCString(name);
	ParticleDefIterator defIt = GetInstance()->m_particleDefs.find(nameHash);

	if (defIt != GetInstance()->m_particleDefs.end()) {
		return defIt->second;
	}
	else {
		return nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleEmitterDefinition* ParticleSystemManager::GetParticleEmitterDefinition(const char* name) {

	size_t nameHash = HashCString(name);
	ParticleEmitterDefMapIterator defIt = GetInstance()->m_emitterDefs.find(nameHash);

	if (defIt != GetInstance()->m_emitterDefs.end()) {
		return defIt->second;
	}
	else {
		return nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleSystemDefinition* ParticleSystemManager::GetParticleSystemDefinition(const char* name) {

	size_t nameHash = HashCString(name);
	ParticleSystemDefIterator  defIt = GetInstance()->m_systemDefs.find(nameHash);

	if (defIt != GetInstance()->m_systemDefs.end()) {
		return defIt->second;
	}
	else {
		return nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void ParticleSystemManager::Shutdown() {

	BeirusEngine::UnregisterUpdateCallback(MakeDelegate(s_particleSystemManager, &ParticleSystemManager::Update));
	delete s_particleSystemManager;
	s_particleSystemManager = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
ParticleSystemManager::~ParticleSystemManager() {

	for (size_t sysIdx = 0; sysIdx < m_pSystems.size(); sysIdx++) {
		delete m_pSystems[sysIdx];
		m_pSystems[sysIdx] = nullptr;
	}

	ParticleDefIterator pDefIt = m_particleDefs.begin();
	for (pDefIt; pDefIt != m_particleDefs.end(); ++pDefIt) {
		delete pDefIt->second;
		pDefIt->second = nullptr;
	}

	ParticleEmitterDefMapIterator pEmDefit = m_emitterDefs.begin();
	for (pEmDefit; pEmDefit != m_emitterDefs.end(); ++pEmDefit) {
		delete pEmDefit->second;
		pEmDefit->second = nullptr;
	}

	ParticleSystemDefIterator pSysDefIt = m_systemDefs.begin();
	for (pSysDefIt;  pSysDefIt != m_systemDefs.end(); ++pSysDefIt) {
		delete pSysDefIt->second;
		pSysDefIt->second = nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleSystemManager* ParticleSystemManager::GetInstance() {

	if (s_particleSystemManager == nullptr) {
		s_particleSystemManager = new ParticleSystemManager();
		BeirusEngine::RegisterUpdateCallback(MakeDelegate(s_particleSystemManager, &ParticleSystemManager::Update));
	}
	return s_particleSystemManager;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ParticleSystemManager::Update(float deltaSeconds) {

	for (uint sysIdx = 0; sysIdx < m_pSystems.size(); sysIdx++) {
		m_pSystems[sysIdx]->Update(deltaSeconds);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//REGISTER UNREGISTER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystemManager::RegisterParticleSystem(ParticleSystem* pSystem) {

	std::vector<ParticleSystem*>* pSystems = &s_particleSystemManager->m_pSystems;

	for (uint defIdx = 0; defIdx < pSystems->size(); defIdx++) {

		if (pSystems->at(defIdx) == pSystem) {
			return false;
		}
	}

	pSystems->push_back(pSystem);
	return true;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystemManager::UnregisterParticleSystem(ParticleSystem* pSystem) {

	std::vector<ParticleSystem*>* pSystems = &s_particleSystemManager->m_pSystems;

	for (uint defIdx = 0; defIdx < pSystems->size(); defIdx++) {

		if (pSystems->at(defIdx) == pSystem) {
			pSystems->erase(pSystems->begin() + defIdx);
			return true;
		}
	}

	return false;
}