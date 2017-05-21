#include "Engine/UI/Widgets/WidgetRegistrationHelper.hpp"
#include "Engine/UI/WidgetBase.hpp"

STATIC std::map<String, WidgetRegistrationHelper*>* WidgetRegistrationHelper::s_widgetClassRegistrationMap = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GENERATOR REGISTRATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
WidgetRegistrationHelper::WidgetRegistrationHelper(const String& name, WidgetCreationFunc* creationFunc, WidgetCreationFuncFromXML* xmlCreationFunc)
	: m_widgetName(name)
	, m_widgetCreationFunc(creationFunc)
	, m_widgetCreationFromXMLFunc(xmlCreationFunc)
{
	if (!s_widgetClassRegistrationMap) {
		s_widgetClassRegistrationMap = new WidgetClassMap;
	}

	WidgetClassMapIterator findGeneratorIt = s_widgetClassRegistrationMap->find(name);

	if (findGeneratorIt == s_widgetClassRegistrationMap->end()) {
		s_widgetClassRegistrationMap->insert(WidgetClassMapPair(name, this));
	}
	else {
		ERROR_AND_DIE(String("ERROR: " + name + "already exists in widget registration map!"));
	}
}