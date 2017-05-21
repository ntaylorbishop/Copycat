#pragma once

#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Renderer/Sprites/SpriteAnim.hpp"
#include "Engine/Renderer/Material/RenderState.hpp"


const int MAX_PDEF_NAME_SIZE = 128;
const char* const PARTICLE_DEF_NODENAME				= "name";
const char* const PDEF_NODE_TEXMAPS					= "TextureMaps";
const char* const PDEF_TEXMAPS_NORMAL				= "normalMap";

const char* const PDEF_BLENDMODE_NODENAME			= "BlendMode";
const char* const PDEF_BLENDMODE_PARAM_OPAQUE		= "opaque";
const char* const PDEF_BLENDMODE_PARAM_TRANSLUCENT	= "translucent";
const char* const PDEF_BLENDMODE_PARAM_ADDITIVE		= "additive";

const char* const PDEF_RPROPS_NODENAME				= "RenderProperties";
const char* const PDEF_RPROP_PARAM_CASTSSHADOWS		= "castsShadows";
const char* const PDEF_RPROP_PARAM_TESTSDEPTH		= "testsDepth";
const char* const PDEF_RPROP_PARAM_WRITESDEPTH		= "writesDepth";
const char* const PDEF_RPROP_PARAM_TRUE				= "true";
const char* const PDEF_RPROP_PARAM_FALSE			= "false";


class ParticleDefinition {
public:
	//LOAD
	static ParticleDefinition* LoadFromXML(const XMLNode& pDefNode);
	static ParticleDefinition* Create() { return new ParticleDefinition(); }

	//STRUCTORS
	ParticleDefinition()	= default;
	~ParticleDefinition()	= default;

	//GETTERS
	const char* GetName()					{ return m_name;		}
	SpriteAnim* GetSpriteAnim()				{ return &m_spriteAnim; }
	void		SetNormalMap(Texture* nMap)	{ m_normalMap = nMap;	}
	Texture*	GetNormalMap()				{ return m_normalMap;	}

	RenderState m_renderState;

private:
	char		m_name[MAX_PDEF_NAME_SIZE]	= "NULL";
	SpriteAnim	m_spriteAnim				= SpriteAnim();
	Texture*	m_normalMap					= nullptr;


	friend class ParticleSystemManager;
	friend class ParticleEmitter;
	friend class Particle;
};