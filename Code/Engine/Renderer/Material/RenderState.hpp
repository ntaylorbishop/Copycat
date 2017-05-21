#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer/RendererCommon.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const eDepthMode	DEFAULT_DEPTH_MODE		= DEPTH_MODE_ON;		//Dual depth testing mode, says if it checks depth at all
const eBlendMode	DEFAULT_BLEND_MODE		= BLEND_MODE_OPAQUE;	//Determines what pass this object is written in
const eCullFace		DEFAULT_CULL_FACE		= CULL_FACE_FRONT;		//Deterimines cull face
const eWritesDepth	DEFAULT_WRITES_DEPTH	= WRITES_DEPTH_ON;		//Determines if depth is written (Useful for SSAO)
const eWritesColor	DEFAULT_WRITES_COLOR	= WRITES_COLOR_ON;		//Determines if color is written
const eCastsShadows	DEFAULT_CASTS_SHADOWS	= CASTS_SHADOWS_ON;		//Deterimines if this material by default casts shadows



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RENDERSTATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class RenderState {
public:
	//STRUCTS
	RenderState() { }
	RenderState(eDepthMode depthMode,		eBlendMode blendMode,		eCullFace cullFace, 
				eWritesDepth writesDepth,	eWritesColor writesColor,	eCastsShadows castsShadows);

	//ENABLE
	void EnableRenderState();



	eDepthMode		m_depthMode			= DEPTH_MODE_DEFAULT;
	eBlendMode		m_blendMode			= BLEND_MODE_DEFAULT;
	eCullFace		m_cullFace			= CULL_FACE_DEFAULT;
	eWritesDepth	m_writesDepth		= WRITES_DEPTH_DEFAULT;
	eWritesColor	m_writesColor		= WRITES_COLOR_DEFAULT;
	eCastsShadows	m_castsShadows		= CASTS_SHADOWS_DEFAULT;
	bool			m_backfaceCulling	= true;


	static const RenderState RENDER_STATE_DEFAULT;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline RenderState::RenderState(eDepthMode depthMode,		eBlendMode blendMode,		eCullFace cullFace,
								eWritesDepth writesDepth,	eWritesColor writesColor,	eCastsShadows castsShadows)
	: m_depthMode(depthMode)
	, m_blendMode(blendMode)
	, m_cullFace(cullFace)
	, m_writesDepth(writesDepth)
	, m_writesColor(writesColor)
	, m_castsShadows(castsShadows)
{ }