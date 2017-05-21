#include "Engine/UI/WidgetGroup.hpp"
#include "Engine/UI/UISystem.hpp"
#include "Engine/UI/WidgetBase.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void WidgetGroup::CreateWidgetGroupFromXML(const XMLNode& widgetGroupNode, WidgetGroup* parentWidgetGroup) {

	WidgetGroup* newWidgetGroup = new WidgetGroup(widgetGroupNode, parentWidgetGroup);
	UISystem::AddWidgetGroup(newWidgetGroup);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
WidgetGroup::WidgetGroup(const XMLNode& widgetGroupNode, WidgetGroup* parentWidgetGroup) {

	UNUSED(parentWidgetGroup);

	String nameAttrib = widgetGroupNode.getAttribute(g_keyInData_Name);
	m_widgetGroupName = nameAttrib;

	NamedProperties* defaultProperties = new NamedProperties();
	m_groupProperties.insert(WidgetGroupPropertiesMapPair(WIDGET_STATE_DEFAULT, defaultProperties));

	for (int widgetGroupAttribIdx = 0; widgetGroupAttribIdx < widgetGroupNode.nAttribute(); widgetGroupAttribIdx++) {
		WidgetBase::SetPropertyFromXML(defaultProperties, widgetGroupNode.getAttribute(widgetGroupAttribIdx), WIDGET_STATE_DEFAULT);
	}


	for (int nodeIdx = 0; nodeIdx < widgetGroupNode.nChildNode(); nodeIdx++) {

		XMLNode childNode = widgetGroupNode.getChildNode(nodeIdx);

		//STATE PROPERTIES
		eWidgetState currStateNode = GetWidgetStateFromString(childNode.getName());

		if (currStateNode == WIDGET_STATE_DEFAULT) {

			for (int stateAttribIdx = 0; stateAttribIdx < childNode.nAttribute(); stateAttribIdx++) {
				WidgetBase::SetPropertyFromXML(defaultProperties, childNode.getAttribute(stateAttribIdx), WIDGET_STATE_DEFAULT);
			}
		}
		else if(currStateNode != WIDGET_STATE_NUM_WIDGET_STATES) {

			NamedProperties* stateProperties = new NamedProperties();
			m_groupProperties.insert(WidgetGroupPropertiesMapPair(currStateNode, stateProperties));

			for (int stateAttribIdx = 0; stateAttribIdx < childNode.nAttribute(); stateAttribIdx++) {
				WidgetBase::SetPropertyFromXML(stateProperties, childNode.getAttribute(stateAttribIdx), currStateNode);
			}
		}
		//WIDGETS
		else if (currStateNode == WIDGET_STATE_NUM_WIDGET_STATES) {
			String widgetName = childNode.getAttribute(g_keyInData_Type);
			WidgetBase* childWidget = WidgetBase::CreateWidgetByName(widgetName, childNode);
			childWidget->SetParent(this);
			m_widgets.push_back(childWidget);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
WidgetGroup::~WidgetGroup() {

	WidgetPropertiesMapIterator propertiesIt = m_groupProperties.begin();

	for (propertiesIt; propertiesIt != m_groupProperties.end(); ++propertiesIt) {
		delete propertiesIt->second;
	}

	for (uint widgetIdx = 0; widgetIdx < m_widgets.size(); widgetIdx++) {
		delete m_widgets[widgetIdx];
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void WidgetGroup::Update(float deltaSeconds) {

	for (uint widgetIdx = 0; widgetIdx < m_widgets.size(); widgetIdx++) {
		m_widgets[widgetIdx]->Update(deltaSeconds);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void WidgetGroup::Render() const {

	for (uint widgetIdx = 0; widgetIdx < m_widgets.size(); widgetIdx++) {
		m_widgets[widgetIdx]->Render();
	}
}