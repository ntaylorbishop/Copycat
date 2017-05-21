#include "Engine/Renderer/Material/RenderState.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

STATIC const RenderState RenderState::RENDER_STATE_DEFAULT(	DEPTH_MODE_ON,		BLEND_MODE_OPAQUE,
															CULL_FACE_FRONT,	WRITES_DEPTH_ON,
															WRITES_COLOR_ON,	CASTS_SHADOWS_ON);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENABLE DISABLE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void RenderState::EnableRenderState() {

	/////////////////////////////////////////////////////////////
	//---------------------------------
	//DEPTH MODE

/**
	eDepthMode mode = m_depthMode;
	if (m_depthMode == DEPTH_MODE_DEFAULT) {
		mode = DEFAULT_DEPTH_MODE;
	}

	switch (mode) {
	case DEPTH_MODE_OFF: {
		BeirusRenderer::DisableDepthTesting();
		break;
	}
	case DEPTH_MODE_ON: {
		BeirusRenderer::EnableDepthTesting();
		break;
	}
	case DEPTH_MODE_DUAL: {
		//TODO: IMPLEMENT DUAL DEPTH TEST MODE (Requires two passes)
		break;
	}
	default: {
		break;
	}
	}
*/

	/////////////////////////////////////////////////////////////
	//---------------------------------
	//BLEND MODE - Opaque is checked in render passes
	eBlendMode blendMode = m_blendMode;
	if (blendMode == BLEND_MODE_DEFAULT) {
		blendMode = DEFAULT_BLEND_MODE;
	}

	switch (blendMode) {

	case BLEND_MODE_TRANSPARENT_ADDITIVE: {
		BeirusRenderer::EnableBlending();
		BeirusRenderer::BlendMode(GL_SRC_ALPHA, GL_ONE);
		break;
	}
	case BLEND_MODE_TRANSPARENT_DEFAULT: {
		BeirusRenderer::EnableBlending();
		BeirusRenderer::BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
	case BLEND_MODE_OPAQUE: {
		BeirusRenderer::DisableBlending();
	}
	//TODO: IMPLEMENT THE OTHER BLEND MODES
	default: {
		break;
	}
	}


/*
	/////////////////////////////////////////////////////////////
	//---------------------------------
	//CULL FACE
	eCullFace cullFace = m_cullFace;
	if (cullFace == CULL_FACE_DEFAULT) {
		cullFace = DEFAULT_CULL_FACE;
	}

	switch (cullFace) {
	case CULL_FACE_FRONT: {
		BeirusRenderer::SetCullFace(true);
		break;
	}
	case CULL_FACE_BACK: {
		BeirusRenderer::SetCullFace(false);
		break;
	}
	default: {
		break;
	}
	}


	/////////////////////////////////////////////////////////////
	//---------------------------------
	//WRITES DEPTH
	eWritesDepth writesDepth = m_writesDepth;
	if (writesDepth == WRITES_DEPTH_DEFAULT) {
		writesDepth = DEFAULT_WRITES_DEPTH;
	}

	switch (writesDepth) {
	case WRITES_DEPTH_ON: {
		BeirusRenderer::EnableDepthWriting();
		break;
	}
	case WRITES_DEPTH_OFF: {
		BeirusRenderer::DisableDepthWriting();
		break;
	}
	default: {
		break;
	}
	}


	/////////////////////////////////////////////////////////////
	//---------------------------------
	//WRITES COLORS
	eWritesColor writesColor = m_writesColor;
	if (writesColor == WRITES_COLOR_DEFAULT) {
		writesColor = DEFAULT_WRITES_COLOR;
	}

	switch (writesColor) {
	case WRITES_COLOR_ON: {
		BeirusRenderer::EnableColorWriting();
		break;
	}
	case WRITES_COLOR_OFF: {
		BeirusRenderer::DisableColorWriting();
		break;
	}
	default: {
		break;
	}
	}
*/
	//Casts shadows is a check when rendering depth
}