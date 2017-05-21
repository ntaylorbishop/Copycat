#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/UI/Utils/UISystemEnums.hpp"

enum eWidgetPropertyType;
class WidgetProperty;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET PROPERTY CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class WidgetProperty {
public:
	//COMPILE-TIME STRUCTORS
	constexpr WidgetProperty(const char* propertyName, eWidgetPropertyType propertyType)
		: m_propertyName(propertyName), m_propertyType(propertyType) { }

	//GETTERS
	const char* GetPropertyName() const { return m_propertyName; }
	eWidgetPropertyType GetPropertyType() const { return m_propertyType; }

	static inline eWidgetPropertyType GetPropertyTypeFromPropertyName(const String& propertyName);

private:

	const char* m_propertyName;
	eWidgetPropertyType m_propertyType;

};

#include "Engine/UI/WidgetProperties.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEMPLATES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC INLINE eWidgetPropertyType WidgetProperty::GetPropertyTypeFromPropertyName(const String& propertyName) {

	for (uint globalWidgetPropertyIdx = 0; globalWidgetPropertyIdx < WidgetPropertiesSize(); globalWidgetPropertyIdx++) {

		if (strcmp(g_widgetProperties[globalWidgetPropertyIdx].GetPropertyName(), propertyName.c_str()) == 0) {

			return g_widgetProperties[globalWidgetPropertyIdx].GetPropertyType();
		}
	}

	return WIDGET_PROPERTY_TYPE_NUM_PROPERTIES;
}