#include "Engine/Particles/ParticleSystemDefinition.hpp"
#include "Engine/Utils/GeneralUtils.hpp"
#include "Engine/Particles/ParticleSystemManager.hpp"
#include "Engine/Particles/ParticleEmitterDefinition.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleSystemDefinition* ParticleSystemDefinition::LoadFromXML(const XMLNode& sysDefNode) {

	ParticleSystemDefinition* newSystemDef = new ParticleSystemDefinition();

	XMLAttribute nameAttrib = sysDefNode.getAttribute(0);

	if (strcmp(nameAttrib.lpszName, PARTICLE_SYS_DEF_NODENAME) == 0) {
		newSystemDef->SetName(nameAttrib.lpszValue);
	}
	else {
		ERROR_AND_DIE("ERROR: First attribute in ParticleSystemDefinition must be definition's name.");
	}

	for (int childNodeIdx = 0; childNodeIdx < sysDefNode.nChildNode(); childNodeIdx++) {

		XMLNode emitterNode = sysDefNode.getChildNode(childNodeIdx);

		if (strcmp(emitterNode.getName(), PARTICLE_SYS_DEF_EMITTERNODE) == 0) {

			ASSERT_OR_DIE(emitterNode.nAttribute() > 0, "ERROR: EmitterDef must have name");
			const char* emitterName = emitterNode.getAttribute(0).lpszValue;
			ParticleEmitterDefinition* emitterDef = ParticleSystemManager::GetParticleEmitterDefinition(emitterName);

			ASSERT_OR_DIE(emitterDef != nullptr, "ERROR: That EmitterDef does not exist.");

			newSystemDef->m_emitterDefs.push_back(emitterDef);
		}
	}

	return newSystemDef;
}


//---------------------------------------------------------------------------------------------------------------------------
bool ParticleSystemDefinition::AddEmitterDef(ParticleEmitterDefinition* emitterDef) {

	for (uint i = 0; i < m_emitterDefs.size(); i++) {

		if (strcmp(emitterDef->GetName(), m_emitterDefs[i]->GetName()) == 0) {
			return false;
		}
	}

	m_emitterDefs.push_back(emitterDef);
	return true;
}