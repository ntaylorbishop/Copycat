#pragma once

#include "Engine/Renderer/General/RGBA.hpp"

const bool	DEFAULT_BACKFACE_CULLING_ENABLED	= false;
const bool	DEFAULT_BLENDING_ENABLED			= false;
const bool	DEFAULT_DEPTH_TESTING_ENABLED		= false;
const bool	DEFAULT_LINE_SMOOTHING_ENABLED		= false;
const bool	DEFAULT_DEPTH_WRITING_ENABLED		= false;
const bool	DEFAULT_COLOR_WRITING_ENABLED		= false;
const RGBA	DEFAULT_CLEAR_SCREEN_COLOR			= RGBA::WHITE;
const float DEFAULT_CLEAR_DEPTH_NUMBER			= 0.f;
const bool	DEFAULT_CULL_FACE_IS_FRONT			= true;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GLOBAL STATE SWITCHING STRUCTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class GlobalStateManager {
public:
	//STRUCTORS
	GlobalStateManager() = default;
	GlobalStateManager(const GlobalStateManager& other) = delete;

	static GlobalStateManager* GetGlobalStateManager() { return &s_globalStateManager; }

	//TOGGLING
	inline bool ShouldToggleBackfaceCulling(bool onOrOff);
	inline bool ShouldToggleBlending(bool onOrOff);
	inline bool ShouldToggleDepthTesting(bool onOrOff);
	inline bool ShouldToggleLineSmoothing(bool onOrOff);
	inline bool ShouldToggleDepthWriting(bool onOrOff);
	inline bool ShouldToggleColorWriting(bool onOrOff);
	
	inline bool ShouldChangeClearColor(const RGBA& clearColor);
	inline bool ShouldChangeClearDepthNumber(float clearDepthNum);
	inline bool ShouldChangeCullFace(bool isFront);

private:
	bool	m_backfaceCullingEnabled	= DEFAULT_BACKFACE_CULLING_ENABLED;
	bool	m_blendingEnabled			= DEFAULT_BLENDING_ENABLED;
	bool	m_depthTestingEnabled		= DEFAULT_DEPTH_TESTING_ENABLED;
	bool	m_lineSmoothingEnabled		= DEFAULT_LINE_SMOOTHING_ENABLED;
	bool	m_depthWritingEnabled		= DEFAULT_DEPTH_WRITING_ENABLED;
	bool	m_colorWritingEnabled		= DEFAULT_COLOR_WRITING_ENABLED;
	RGBA	m_clearScreenColor			= DEFAULT_CLEAR_SCREEN_COLOR;
	float	m_clearDepthNumber			= DEFAULT_CLEAR_DEPTH_NUMBER;
	bool	m_cullFaceIsFront			= DEFAULT_CULL_FACE_IS_FRONT;


	static GlobalStateManager s_globalStateManager;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldToggleBackfaceCulling(bool onOrOff) {
	if (onOrOff != m_backfaceCullingEnabled) {
		m_backfaceCullingEnabled = !m_backfaceCullingEnabled;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldToggleBlending(bool onOrOff) {
	if (onOrOff != m_blendingEnabled) {
		m_blendingEnabled = !m_blendingEnabled;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldToggleDepthTesting(bool onOrOff) {
	if (onOrOff != m_depthTestingEnabled) {
		m_depthTestingEnabled = !m_depthTestingEnabled;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldToggleLineSmoothing(bool onOrOff) {
	if (onOrOff != m_lineSmoothingEnabled) {
		m_lineSmoothingEnabled = !m_lineSmoothingEnabled;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldToggleDepthWriting(bool onOrOff) {
	if (onOrOff != m_depthWritingEnabled) {
		m_depthWritingEnabled = !m_depthWritingEnabled;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldToggleColorWriting(bool onOrOff) {
	if (onOrOff != m_colorWritingEnabled) {
		m_colorWritingEnabled = !m_colorWritingEnabled;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldChangeClearColor(const RGBA& clearColor) {
	if (clearColor != m_clearScreenColor) {
		m_clearScreenColor = clearColor;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldChangeClearDepthNumber(float clearDepthNum) {
	if (clearDepthNum != m_clearDepthNumber) {
		m_clearDepthNumber = clearDepthNum;
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool GlobalStateManager::ShouldChangeCullFace(bool isFront) {
	if (isFront != m_cullFaceIsFront) {
		m_cullFaceIsFront = isFront;
		return true;
	}
	return false;
}