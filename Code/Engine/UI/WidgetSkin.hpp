#pragma once

#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/UI/UISystemCommon.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

class WidgetBase;
enum eWidgetType;
enum eWidgetState;

typedef std::pair<eWidgetType, eWidgetState> PropertyMapKey;

class WidgetSkin {
public:
	//CREATION
	static WidgetSkin* CreateSkin(const XMLNode& node);

	//GETTERS SETTERS
	String GetName() const { return m_skinName; }


private:
	//STRUCTORS
	WidgetSkin(const String& name) : m_skinName(name) { }
	~WidgetSkin();

	//ADDING PROPERTIES
	void AddAllWidgetsProperties(const XMLNode& allWidgetPropertyNodes);
	void AddWidgetSpecificProperties(const XMLNode& widgetPropertyNodes, eWidgetType widgetType);

	//PRIVATE GETTERS
	NamedProperties* GetPropertiesForWidgetTypeAndState(eWidgetType widgetType, eWidgetState widgetState);
	inline NamedProperties* GetAllWidgetAllStatesProperties();


	String m_skinName;
	std::map<PropertyMapKey, NamedProperties*> m_skinProperties;

	friend class WidgetBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef std::map<PropertyMapKey, NamedProperties*>				PropertyMap;
typedef std::map<PropertyMapKey,	NamedProperties*>::iterator	PropertyMapIterator;
typedef std::pair<PropertyMapKey,	NamedProperties*>			PropertyMapPair;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
INLINE NamedProperties* WidgetSkin::GetAllWidgetAllStatesProperties() {

	PropertyMapIterator mapIt = m_skinProperties.find(PropertyMapKey(WIDGET_TYPE_ALL, WIDGET_STATE_DEFAULT));
	return mapIt->second;
}