#include "Engine/Particles/ParticleDefinition.hpp"
#include "Engine/Particles/ParticleEmitterDefinition.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOAD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static void ParseTextureMaps(ParticleDefinition* pDef, const XMLNode& texMapNode) {

	for (int attribIdx = 0; attribIdx < texMapNode.nAttribute(); attribIdx++) {
		
		const char* attribName	= texMapNode.getAttribute(attribIdx).lpszName;
		const char* attribVal	= texMapNode.getAttribute(attribIdx).lpszValue;

		if (strcmp(attribName, PDEF_TEXMAPS_NORMAL) == 0) {
			Texture* normalMap = Texture::CreateOrGetTexture(attribVal);
			pDef->SetNormalMap(normalMap);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseRenderProperties(ParticleDefinition* pDef, const XMLNode& rPropNode) {

	for (int attribIdx = 0; attribIdx < rPropNode.nAttribute(); attribIdx++) {

		const char* attribName = rPropNode.getAttribute(attribIdx).lpszName;
		const char* attribVal = rPropNode.getAttribute(attribIdx).lpszValue;

		if (strcmp(attribName, PDEF_BLENDMODE_NODENAME) == 0) {
			
			if (strcmp(attribVal, PDEF_BLENDMODE_PARAM_OPAQUE) == 0) {
				pDef->m_renderState.m_blendMode = BLEND_MODE_OPAQUE;
			}
			else if (strcmp(attribVal, PDEF_BLENDMODE_PARAM_TRANSLUCENT) == 0) {
				pDef->m_renderState.m_blendMode = BLEND_MODE_TRANSPARENT_DEFAULT;
			}
			else if (strcmp(attribVal, PDEF_BLENDMODE_PARAM_ADDITIVE) == 0) {
				pDef->m_renderState.m_blendMode = BLEND_MODE_TRANSPARENT_ADDITIVE;
			}
		}
		else if (strcmp(attribName, PDEF_RPROP_PARAM_CASTSSHADOWS) == 0) {

			if (strcmp(attribVal, PDEF_RPROP_PARAM_TRUE) == 0) {
				pDef->m_renderState.m_castsShadows = CASTS_SHADOWS_ON;
			}
			else if (strcmp(attribVal, PDEF_RPROP_PARAM_FALSE) == 0) {
				pDef->m_renderState.m_castsShadows = CASTS_SHADOWS_OFF;
			}
		}
		else if (strcmp(attribName, PDEF_RPROP_PARAM_TESTSDEPTH) == 0) {

			if (strcmp(attribVal, PDEF_RPROP_PARAM_TRUE) == 0) {
				pDef->m_renderState.m_depthMode = DEPTH_MODE_ON;
			}
			else if (strcmp(attribVal, PDEF_RPROP_PARAM_FALSE) == 0) {
				pDef->m_renderState.m_depthMode = DEPTH_MODE_OFF;
			}
		}
		else if (strcmp(attribName, PDEF_RPROP_PARAM_WRITESDEPTH) == 0) {

			if (strcmp(attribVal, PDEF_RPROP_PARAM_TRUE) == 0) {
				pDef->m_renderState.m_writesDepth = WRITES_DEPTH_ON;
			}
			else if (strcmp(attribVal, PDEF_RPROP_PARAM_FALSE) == 0) {
				pDef->m_renderState.m_writesDepth = WRITES_DEPTH_OFF;
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleDefinition* ParticleDefinition::LoadFromXML(const XMLNode& pDefNode) {

	ParticleDefinition* newPDef = new ParticleDefinition();

	//SET NAME
	XMLAttribute nameAttrib = pDefNode.getAttribute(0);
	ASSERT_OR_DIE(strcmp(nameAttrib.lpszName, PARTICLE_DEF_NODENAME) == 0, "ERROR: ParticleDefinition must have name as first attribute.");
	const char* pDefName = nameAttrib.lpszValue;
	strcpy_s(newPDef->m_name, MAX_PDEF_NAME_SIZE, pDefName);

	//PARSE ANIMS
	for (int nodeIdx = 0; nodeIdx < pDefNode.nChildNode(); nodeIdx++) {

		XMLNode currNode = pDefNode.getChildNode(nodeIdx);
		if (strcmp(currNode.getName(), SPRITE_ANIM_XML_NAME) == 0) {
			newPDef->m_spriteAnim = SpriteAnim::LoadFromXML(currNode);
		}
		else if (strcmp(currNode.getName(), PDEF_NODE_TEXMAPS) == 0) {
			ParseTextureMaps(newPDef, currNode);
		}
		else if (strcmp(currNode.getName(), PDEF_RPROPS_NODENAME) == 0) {
			ParseRenderProperties(newPDef, currNode);
		}
	}

	return newPDef;
}