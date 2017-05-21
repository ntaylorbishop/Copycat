#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//RENDER STATE ENUMS
//---------------------------------------------------------------------------------------------------------------------------
enum eDepthMode : uint8_t {
	DEPTH_MODE_DEFAULT = 0,
	DEPTH_MODE_ON,
	DEPTH_MODE_OFF,
	DEPTH_MODE_DUAL
};

enum eBlendMode : uint8_t {
	BLEND_MODE_DEFAULT = 0,
	BLEND_MODE_OPAQUE,
	BLEND_MODE_TRANSPARENT_ADDITIVE,
	BLEND_MODE_TRANSPARENT_DEFAULT,
	BLEND_MODE_TRANSPARENT_MIN,
	BLEND_MODE_TRANSPARENT_MAX
};

enum eCullFace : uint8_t {
	CULL_FACE_DEFAULT = 0,
	CULL_FACE_FRONT,
	CULL_FACE_BACK
};

enum eWritesDepth : uint8_t {
	WRITES_DEPTH_DEFAULT = 0,
	WRITES_DEPTH_ON,
	WRITES_DEPTH_OFF
};

enum eWritesColor : uint8_t {
	WRITES_COLOR_DEFAULT = 0,
	WRITES_COLOR_ON,
	WRITES_COLOR_OFF
};

enum eCastsShadows : uint8_t {
	CASTS_SHADOWS_DEFAULT = 0,
	CASTS_SHADOWS_ON,
	CASTS_SHADOWS_OFF
};