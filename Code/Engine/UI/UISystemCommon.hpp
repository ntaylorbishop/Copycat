#pragma once

#include "Engine/UI/Utils/UISystemEnums.hpp"

eWidgetState GetWidgetStateFromString(const String& widgetStateStr);

const char* const g_keyInData_AllWidgets = "AllWidgets";
const char* const g_keyInData_Widget = "Widget";
const char* const g_keyInData_Name = "name";
const char* const g_keyInData_Type = "type";
const char* const g_keyInData_Skin = "skin";
const char* const g_keyInData_SkinSpecifier = "Skin";
const char* const g_keyInData_UIWidget = "UIWidget";
const char* const g_keyInData_WidgetGroup = "WidgetGroup";
const char* const g_keyInData_Skins = "Skins";
const char* const g_keyInData_UIWidgets = "UIWidgets";
const char* const g_keyInData_PropertyAnims = "WidgetAnimations";
const char* const g_keyInData_PropertyAnim = "AnimatedProperty";
const char* const g_keyInData_NoEventAssociated = "NULL";

const char* const g_keyInData_animPropertyName = "name";
const char* const g_keyInData_keyframeName = "Keyframe";
const char* const g_keyInData_keyframeTime = "time";

const char* const g_keyInData_animModeLoop = "loop";
const char* const g_keyInData_animModePingPong = "pingPong";
const char* const g_keyInData_animModeClamp = "clamp";