#pragma once

#include "Engine/UI/Utils/UISystemEnums.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET PROPERTIES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr WidgetProperty g_widgetProperties[] = {
	WidgetProperty("width",				WIDGET_PROPERTY_TYPE_UINT),
	WidgetProperty("height",			WIDGET_PROPERTY_TYPE_UINT),
	WidgetProperty("minWidth",			WIDGET_PROPERTY_TYPE_UINT),
	WidgetProperty("minHeight",			WIDGET_PROPERTY_TYPE_UINT),
	WidgetProperty("maxWidth",			WIDGET_PROPERTY_TYPE_UINT),
	WidgetProperty("maxHeight",			WIDGET_PROPERTY_TYPE_UINT),
	WidgetProperty("name",				WIDGET_PROPERTY_TYPE_STRING),
	WidgetProperty("absolutePosition",	WIDGET_PROPERTY_TYPE_INTVECTOR2),
	WidgetProperty("relativePosition",	WIDGET_PROPERTY_TYPE_INTVECTOR2),
	WidgetProperty("skin",				WIDGET_PROPERTY_TYPE_SKIN),
	WidgetProperty("text",				WIDGET_PROPERTY_TYPE_STRING),
	WidgetProperty("radioGrouping",		WIDGET_PROPERTY_TYPE_RADIOGROUPING),
	WidgetProperty("radioTextAlign",	WIDGET_PROPERTY_TYPE_RADIOTEXTALIGN),
	WidgetProperty("fontSize",			WIDGET_PROPERTY_TYPE_FLOAT),
	WidgetProperty("fontColor",			WIDGET_PROPERTY_TYPE_COLOR),
	WidgetProperty("backgroundImg",		WIDGET_PROPERTY_TYPE_TEXTURE),
	WidgetProperty("backgroundImgOn",	WIDGET_PROPERTY_TYPE_TEXTURE),
	WidgetProperty("backgroundImgOff",	WIDGET_PROPERTY_TYPE_TEXTURE),
	WidgetProperty("width",				WIDGET_PROPERTY_TYPE_FLOAT),
	WidgetProperty("fontType",			WIDGET_PROPERTY_TYPE_FONT),
	WidgetProperty("padding-left",		WIDGET_PROPERTY_TYPE_INT),
	WidgetProperty("padding-right",		WIDGET_PROPERTY_TYPE_INT),
	WidgetProperty("padding-top",		WIDGET_PROPERTY_TYPE_INT),
	WidgetProperty("padding-bottom",	WIDGET_PROPERTY_TYPE_INT),
	WidgetProperty("padding",			WIDGET_PROPERTY_TYPE_INT),
	WidgetProperty("onClick",			WIDGET_PROPERTY_EVENT_ONCLICK),
	WidgetProperty("onHover",			WIDGET_PROPERTY_EVENT_ONHOVER),
	WidgetProperty("onPressed",			WIDGET_PROPERTY_EVENT_ONPRESSED),
	WidgetProperty("onHoverAnim",		WIDGET_PROPERTY_ANIM_ONHOVER),
	WidgetProperty("onClickAnim",		WIDGET_PROPERTY_ANIM_ONCLICK),
	WidgetProperty("onValueAnim",		WIDGET_PROPERTY_ANIM_ONVALUE),
	WidgetProperty("onPressedAnim",		WIDGET_PROPERTY_ANIM_ONPRESSED)
};

constexpr uint WidgetPropertiesSize() { return sizeof(g_widgetProperties) / sizeof(g_widgetProperties[0]); }