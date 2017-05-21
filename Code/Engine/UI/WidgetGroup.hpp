#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/UI/Utils/UISystemEnums.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

class WidgetBase;

class WidgetGroup {
public:
	//CREATION
	static void CreateWidgetGroupFromXML(const XMLNode& widgetGroupNode, WidgetGroup* parentWidgetGroup);

	//STRUCTORS
	WidgetGroup(const XMLNode& widgetGroupNode, WidgetGroup* parentWidgetGroup);
	WidgetGroup() = default;
	~WidgetGroup();

	//WIDGETS
	void AddWidget(WidgetBase* newWidget) { m_widgets.push_back(newWidget); }


	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

private:
	//GET PROPERTY
	template <typename T>
	bool GetProperty(const String& propertyName, eWidgetState widgetState, T& outProperty) const;

	bool m_isHidden = false;

	std::vector<WidgetBase*> m_widgets;

	String m_widgetGroupName;
	std::map<eWidgetState, NamedProperties*> m_groupProperties;

	friend class WidgetBase;
	friend class RadioWidget;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef std::map<eWidgetState, NamedProperties*>::const_iterator	WidgetGroupPropertiesMapConstIterator;
typedef std::map<eWidgetState, NamedProperties*>::iterator			WidgetGroupPropertiesMapIterator;
typedef std::pair<eWidgetState, NamedProperties*>					WidgetGroupPropertiesMapPair;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GET PROPERTY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//GET PROPERTY
template <typename T>
bool WidgetGroup::GetProperty(const String& propertyName, eWidgetState widgetState, T& outProperty) const {

	WidgetGroupPropertiesMapConstIterator propertiesIt = m_groupProperties.find(widgetState);

	if (propertiesIt != m_groupProperties.end()) {

		NamedProperties* stateSpecificProperties = propertiesIt->second;
		ePropertyGetResult didGetProperty = stateSpecificProperties->GetProperty(propertyName, outProperty);

		if (didGetProperty == PGR_SUCCESS) {
			return true;
		}
	}

	propertiesIt = m_groupProperties.find(WIDGET_STATE_DEFAULT);
	ASSERT_OR_DIE(propertiesIt != m_groupProperties.end(), "ERROR: No default properties defined for WidgetGroup.");

	NamedProperties* defaultProperties = propertiesIt->second;
	ePropertyGetResult didGetProperty = defaultProperties->GetProperty(propertyName, outProperty);

	if (didGetProperty == PGR_SUCCESS) {
		return true;
	}

	return false;
}