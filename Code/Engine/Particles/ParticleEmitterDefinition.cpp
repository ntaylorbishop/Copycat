#include "Engine/Particles/ParticleEmitterDefinition.hpp"
#include "Engine/Particles/ParticleDefinition.hpp"
#include "Engine/Particles/ParticleSystemManager.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOAD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleEmitterDefinition* ParticleEmitterDefinition::LoadFromXML(const XMLNode& emitterDefNode) {

	ParticleEmitterDefinition* newEmitterDef = new ParticleEmitterDefinition();

	for (int attribIdx = 0; attribIdx < emitterDefNode.nAttribute(); attribIdx++) {

		XMLAttribute attrib = emitterDefNode.getAttribute(attribIdx);
		const char* attribName = attrib.lpszName;
		const char* attribVal = attrib.lpszValue;

		if (strcmp(attribName, PEMITTERDEF_ATTRIB_NAME) == 0) {
			strcpy_s(newEmitterDef->m_name, MAX_PARTICLE_EMITTER_NAME_SIZE, attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_INITSPAWNCOUNT) == 0) {
			newEmitterDef->m_initialSpawnCount = XMLUtils::ParseRangeInt(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_SPAWNRATE) == 0) {
			newEmitterDef->m_spawnRate = XMLUtils::ParseFloat(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_LIFETIME) == 0) {
			newEmitterDef->m_lifetime = XMLUtils::ParseRangeFloat(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_INITSCALE) == 0) {
			newEmitterDef->m_initialScale = XMLUtils::ParseRangeFloat(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_COLOR) == 0) {
			newEmitterDef->m_tint = XMLUtils::ParseHexColor(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_STARTVEL) == 0) {
			newEmitterDef->m_startVelocity = XMLUtils::ParseRangeVector3(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_VELSTR) == 0) {
			newEmitterDef->m_velocityStrength = XMLUtils::ParseRangeFloat(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_ACCEL) == 0) {
			newEmitterDef->m_acceleration = XMLUtils::ParseRangeVector3(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_SHADERTYPE) == 0) {
			newEmitterDef->SetShaderName(attribVal);
		}
		else if (strcmp(attribName, PEMITTERDEF_ATTRIB_PDEF) == 0) {
			newEmitterDef->m_particleDef = ParticleSystemManager::GetParticleDefinition(attribVal);
		}
	}

	ASSERT_OR_DIE(strcmp(newEmitterDef->m_name, "NULL") != 0, "ERROR: Name must be defined for particle emitter definition XML node.");
	return newEmitterDef;
}