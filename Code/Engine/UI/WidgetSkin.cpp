#include "Engine/UI/WidgetSkin.hpp"
#include "Engine/UI/WidgetBase.hpp"
#include "Engine/UI/UISystem.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC CREATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetSkin* WidgetSkin::CreateSkin(const XMLNode& skinNode) {

	String skinName = skinNode.getAttributeValue(0);
	WidgetSkin* newSkin = new WidgetSkin(skinName);

	for (int i = 0; i < skinNode.nChildNode(); i++) {

		XMLNode widgetTypeNode = skinNode.getChildNode(i);

		////////////////////////////////////////////////////
		//------------------------------------------
		//ALL-WIDGETS
		if (strcmp(widgetTypeNode.getName(), g_keyInData_AllWidgets) == 0) {
			newSkin->AddAllWidgetsProperties(widgetTypeNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//BUTTONS
		if (strcmp(widgetTypeNode.getName(), g_keyInData_Widget) == 0) {
			newSkin->AddWidgetSpecificProperties(widgetTypeNode, WidgetBase::GetWidgetTypeFromString(widgetTypeNode.getAttributeValue(0)));
		}
	}
	
	UISystem::AddSkin(newSkin);
	return newSkin;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADDING PROPERTIES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void WidgetSkin::AddAllWidgetsProperties(const XMLNode& allWidgetPropertyNodes) {

	NamedProperties* allProperties = new NamedProperties();
	m_skinProperties.insert(PropertyMapPair(PropertyMapKey(WIDGET_TYPE_ALL, WIDGET_STATE_DEFAULT), allProperties));

	for (int i = 0; i < allWidgetPropertyNodes.nAttribute(); i++) {
		WidgetBase::SetPropertyFromXML(allProperties, allWidgetPropertyNodes.getAttribute(i), WIDGET_STATE_DEFAULT);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void WidgetSkin::AddWidgetSpecificProperties(const XMLNode& widgetPropertyNodes, eWidgetType widgetType) {

	for (int stateIdx = 0; stateIdx < widgetPropertyNodes.nChildNode(); stateIdx++) {

		XMLNode stateNode = widgetPropertyNodes.getChildNode(stateIdx);

		eWidgetState widgetStateNode = GetWidgetStateFromString(stateNode.getName());
		NamedProperties* stateProperties = new NamedProperties();
		m_skinProperties.insert(PropertyMapPair(PropertyMapKey(widgetType, widgetStateNode), stateProperties));

		for (int attribIdx = 0; attribIdx < stateNode.nAttribute(); attribIdx++) {
			WidgetBase::SetPropertyFromXML(stateProperties, stateNode.getAttribute(attribIdx), widgetStateNode);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
WidgetSkin::~WidgetSkin() {

	PropertyMapIterator propertyIt = m_skinProperties.begin();

	for (propertyIt; propertyIt != m_skinProperties.end(); ++propertyIt) {
		delete propertyIt->second;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE GETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
NamedProperties* WidgetSkin::GetPropertiesForWidgetTypeAndState(eWidgetType widgetType, eWidgetState widgetState) {

	PropertyMapKey propertyMapKey = PropertyMapKey(widgetType, widgetState);
	PropertyMapIterator propertyMapIt = m_skinProperties.find(propertyMapKey);

	NamedProperties* properties = nullptr;

	if (propertyMapIt != m_skinProperties.end()) {
		properties = propertyMapIt->second;
	}

	return properties;
}