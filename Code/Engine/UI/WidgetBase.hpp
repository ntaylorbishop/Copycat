#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/Fonts/Font.hpp"
#include "Engine/UI/WidgetSkin.hpp"
#include "Engine/UI/WidgetGroup.hpp"
#include "Engine/UI/Utils/UISystemEnums.hpp"
#include "Engine/UI/Utils/WidgetProperty.hpp"
#include "Engine/UI/UISystemCommon.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

class WidgetSkin;
class WidgetGroup;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGETBASE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class WidgetBase {
public:
	//CREATION
	static WidgetBase* CreateWidgetByName(const String& name, const IntVector2& position);
	static WidgetBase* CreateWidgetByName(const String& name, const XMLNode& widgetData);

	//UPDATE RENDER
	virtual void Update(float deltaSeconds);
	virtual void Render() const;

	//EVENT FIRING
	virtual void DoOnClickStuff() { }

	//SET PROPERTIES
	static void SetPropertyFromXML(NamedProperties* properties, const XMLAttribute& attrib, eWidgetState currStateNode); //NEW PROPERTIES NEED TO ADD TO HERE

	//GET PROPERTIES
	virtual IntVector2 GetResolvedPosition() const;
	virtual IntVector2 GetResolvedSize() const;
	virtual AABB2 GetContainingQuad() const;
	eWidgetType GetWidgetType() const { return m_widgetType; }

	void SetWidgetState(eWidgetState state) { m_currentState = state; }
	void SetParent(WidgetGroup* parent) { m_parent = parent; }

	//HIDE SHOW
	void Hide() { m_isHidden = true;	}
	void Show() { m_isHidden = false;	}

	bool operator==(const WidgetBase& other) const { return m_widgetID == other.m_widgetID; }
	bool operator!=(const WidgetBase& other) const { return m_widgetID != other.m_widgetID; }


protected:
	//STRUCTORS
	WidgetBase(WidgetGroup* belongingWidgetGroup, eWidgetType widgetType, eWidgetState widgetState);
	virtual ~WidgetBase();

	//INIT
public:
	void Initialize(const IntVector2& position);
	void Initialize(const XMLNode& widgetData);
	void InitializeFromXML(const XMLNode& widgetNode);
	void InitializeDefaultProperties();
	virtual void InitializeWidgetSpecificDefaultPropertiesAndEvents() { }

	//PROTECTED GETTERS
	template<typename T>
	bool GetProperty(const String& propertyName, T& out) const;

protected:
	//UPDATE: STATE SWITCHING
	bool CheckIfHovered();
	bool CheckIfPressed();
	bool CheckIfClicked();

	void FireAssociatedEvent(const String& eventName);


	template<typename T>
	bool GetPropertyFromParent(const String& propertyName, T& out) const;
	
	static inline eWidgetType GetWidgetTypeFromString(const String& widgetTypeStr);

	NamedProperties* GetWidgetPropertiesForState(eWidgetState widgetState);

	WidgetSkin* m_skin;
	std::map<eWidgetState, NamedProperties*> m_codeDefaultWidgetProperties;
	std::map<eWidgetState, NamedProperties*> m_widgetProperties;

	String m_name;
	WidgetGroup* m_parent;
	eWidgetType m_widgetType;

	eWidgetState m_currentState;
	bool m_isSelected = false;
	bool m_isHidden = false;
	size_t m_widgetID;

	static size_t s_widgetID; 


	friend class WidgetGroup;
	friend class WidgetSkin;
	friend class UISystem;
	friend class WidgetRegistrationHelper;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef std::map<eWidgetState, NamedProperties*>::const_iterator	WidgetPropertiesMapConstIterator;
typedef std::map<eWidgetState, NamedProperties*>::iterator			WidgetPropertiesMapIterator;
typedef std::pair<eWidgetState, NamedProperties*>					WidgetPropertiesMapPair;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

STATIC INLINE eWidgetType WidgetBase::GetWidgetTypeFromString(const String& widgetTypeStr) {

	if (widgetTypeStr == "Button") {
		return WIDGET_TYPE_BUTTON;
	}
	else if (widgetTypeStr == "Label") {
		return WIDGET_TYPE_LABEL;
	}
	else if (widgetTypeStr == "Container") {
		return WIDGET_TYPE_CONTAINER;
	}
	else if (widgetTypeStr == "Radio") {
		return WIDGET_TYPE_RADIO;
	}
	else {
		return WIDGET_TYPE_ALL;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GET PROPERTY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template<typename T>
bool WidgetBase::GetProperty(const String& propertyName, T& out) const {

	//TRY GET FROM WIDGET PROPERTIES STATE-SPECIFIC
	WidgetPropertiesMapConstIterator stateProperties = m_widgetProperties.find(m_currentState);
	
	if (stateProperties != m_widgetProperties.end()) {

		NamedProperties* widgetProperties = stateProperties->second;
		ePropertyGetResult didGetProperty = widgetProperties->GetProperty(propertyName, out);

		if (didGetProperty == PGR_SUCCESS) {
			return true;
		}
	}
	
	//TRY GET FROM WIDGET PROPERTIES ALL-STATES
	WidgetPropertiesMapConstIterator allStateProperties = m_widgetProperties.find(WIDGET_STATE_DEFAULT);

	if (allStateProperties != m_widgetProperties.end()) {

		NamedProperties* widgetProperties = allStateProperties->second;
		ePropertyGetResult didGetProperty = widgetProperties->GetProperty(propertyName, out);

		if (didGetProperty == PGR_SUCCESS) {
			return true;
		}
	}

	//TRY GET FROM SKIN WIDGET-SPECIFIC STATE-SPECIFIC
	if (m_skin != nullptr) {
		NamedProperties* widgetSpecificStateSpecificProperties = m_skin->GetPropertiesForWidgetTypeAndState(m_widgetType, m_currentState);

		if (widgetSpecificStateSpecificProperties != nullptr) {

			ePropertyGetResult didGetProperty = widgetSpecificStateSpecificProperties->GetProperty(propertyName, out);

			if (didGetProperty == PGR_SUCCESS) {
				return true;
			}
		}
	}

	//TRY GET FROM SKIN WIDGET-SPECIFIC
	if (m_skin != nullptr) {
		NamedProperties* widgetSpecificProperties = m_skin->GetPropertiesForWidgetTypeAndState(m_widgetType, WIDGET_STATE_DEFAULT);

		if (widgetSpecificProperties != nullptr) {

			ePropertyGetResult didGetProperty = widgetSpecificProperties->GetProperty(propertyName, out);

			if (didGetProperty == PGR_SUCCESS) {
				return true;
			}
		}
	}

	//TRY GET FROM SKIN ALL WIDGETS
	if (m_skin != nullptr) {
		NamedProperties* allWidgetProperties = m_skin->GetAllWidgetAllStatesProperties();

		if (allWidgetProperties != nullptr) {

			ePropertyGetResult didGetProperty = allWidgetProperties->GetProperty(propertyName, out);

			if (didGetProperty == PGR_SUCCESS) {
				return true;
			}
		}
	}

	//TRY GET FROM PARENT WIDGET
	if (m_parent != nullptr) {

		bool gotPropertyFromParent = m_parent->GetProperty(propertyName, m_currentState, out);

		if (gotPropertyFromParent) {
			return true;
		}
	}
	
	//GET FROM CODE DEFAULTS STATE-SPECIFIC
	WidgetPropertiesMapConstIterator stateDefaultPropertiesIt = m_codeDefaultWidgetProperties.find(m_currentState);

	if (stateDefaultPropertiesIt != m_codeDefaultWidgetProperties.end()) {

		NamedProperties* stateDefaultProperties = const_cast<NamedProperties*>(stateDefaultPropertiesIt->second);
		ePropertyGetResult didGetProperty = stateDefaultProperties->GetProperty(propertyName, out);

		if (didGetProperty == PGR_SUCCESS) {
			return true;
		}
	}

	//GET FROM CODE DEFAULTS ALL STATES
	WidgetPropertiesMapConstIterator defaultPropertiesIt = m_codeDefaultWidgetProperties.find(WIDGET_STATE_DEFAULT);

	if (defaultPropertiesIt != m_codeDefaultWidgetProperties.end()) {

		NamedProperties* defaultProperties = const_cast<NamedProperties*>(defaultPropertiesIt->second);
		ePropertyGetResult didGetProperty = defaultProperties->GetProperty(propertyName, out);

		if (didGetProperty == PGR_SUCCESS) {
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
template<typename T>
bool WidgetBase::GetPropertyFromParent(const String& propertyName, T& out) const {
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NOTES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	STATES
		Disabled
		Hover
		Enabled
		Pressed
		Selected
		Hidden
*/

/*
	LAYERS

		TYPE 		WHERE DECLARE		STATE TYPE 			PRIORITY
		C++ 		All-Widgets 		All-States			12
		C++ 		All-Widgets 		State-Specifics 	9
		C++ 		Widget-Specific 	All States 			6
		C++ 		Widget-Specific 	State-Specific 		3
		Skin 		All-Widgets 		All States 			11
		Skin 		All-Widgets 		State-Specific 		8
		Skin		Widget-Specific 	All States 			5
		Skin		Widget-Specific 	State-Specific 		2
		Widget 		All-Widgets 		All States 			10
		Widget 		All-Widgets 		State-Specific 		7
		Widget 		Widget-Specific 	All States 			4
		Widget 		Widget-Specific		State-Specific 		1
*/


/*
	PROPERTY NAMES
		"fontType"
		"fontColor"
		"fontSize"
		"width"
		"height"
		"maxWidth"
		"maxHeight"
		"minWidth"
		"minHeight"
		"backgroundColor"
		"borderColor"
		"backgroundImg"
*/