#pragma once

#include "Engine/General/Core/EngineCommon.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum eWidgetState {
	WIDGET_STATE_ENABLED = 0,
	WIDGET_STATE_DISABLED,
	WIDGET_STATE_PRESSED,
	WIDGET_STATE_HOVER,
	WIDGET_STATE_ENABLED_SELECTED,
	WIDGET_STATE_DISABLED_SELECTED,
	WIDGET_STATE_HOVER_SELECTED,
	WIDGET_STATE_PRESSED_SELECTED,
	WIDGET_STATE_DEFAULT,
	WIDGET_STATE_NUM_WIDGET_STATES
};


//---------------------------------------------------------------------------------------------------------------------------
enum eWidgetType {
	WIDGET_TYPE_BUTTON = 0,
	WIDGET_TYPE_LABEL,
	WIDGET_TYPE_CONTAINER,
	WIDGET_TYPE_RADIO,
	WIDGET_TYPE_ALL
};


//---------------------------------------------------------------------------------------------------------------------------
enum eWidgetProperty {
	WIDGET_PROPERTY_NAME = 0,
	WIDGET_PROPERTY_ABSOLUTE_POSITION,
	WIDGET_PROPERTY_
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET PROPERTY ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum eWidgetPropertyType {
	WIDGET_PROPERTY_TYPE_FLOAT = 0,
	WIDGET_PROPERTY_TYPE_COLOR,
	WIDGET_PROPERTY_TYPE_STRING,
	WIDGET_PROPERTY_TYPE_INTVECTOR2,
	WIDGET_PROPERTY_TYPE_VECTOR2,
	WIDGET_PROPERTY_TYPE_INT,
	WIDGET_PROPERTY_TYPE_UINT,
	WIDGET_PROPERTY_TYPE_TEXTURE,
	WIDGET_PROPERTY_TYPE_FONT,
	WIDGET_PROPERTY_TYPE_RADIOGROUPING,
	WIDGET_PROPERTY_TYPE_RADIOTEXTALIGN,
	WIDGET_PROPERTY_TYPE_SKIN,
	WIDGET_PROPERTY_EVENT_ONCLICK,
	WIDGET_PROPERTY_EVENT_ONVALUE,
	WIDGET_PROPERTY_EVENT_ONHOVER,
	WIDGET_PROPERTY_EVENT_ONPRESSED,
	WIDGET_PROPERTY_ANIM_ONHOVER,
	WIDGET_PROPERTY_ANIM_ONCLICK,
	WIDGET_PROPERTY_ANIM_ONVALUE,
	WIDGET_PROPERTY_ANIM_ONPRESSED,
	WIDGET_PROPERTY_TYPE_NUM_PROPERTIES
};


enum eWidgetAnimatedPropertyType {
	WIDGET_ANIMATED_PROPERTY_TYPE_FLOAT = 0,
	WIDGET_ANIMATED_PROPERTY_TYPE_INTVECTOR2,
	WIDGET_ANIMATED_PROPERTY_TYPE_RGBA,
	WIDGET_ANIMATED_PROPERTY_TYPE_INT
};