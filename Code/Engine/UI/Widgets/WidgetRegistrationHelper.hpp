#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/UI/WidgetBase.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

typedef WidgetBase* (WidgetCreationFunc)(const IntVector2& absolutePosition);
typedef WidgetBase* (WidgetCreationFuncFromXML)(const XMLNode& widgetData);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET REGISTRATION HELPER CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WidgetRegistrationHelper {
public:
	WidgetRegistrationHelper(const String& name, WidgetCreationFunc* creationFunc, WidgetCreationFuncFromXML* xmlCreationFunc);

public:
	String m_widgetName;
	WidgetCreationFunc* m_widgetCreationFunc;
	WidgetCreationFuncFromXML* m_widgetCreationFromXMLFunc;

	static std::map<String, WidgetRegistrationHelper*>* s_widgetClassRegistrationMap;

	friend class UISystem;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef std::map<String, WidgetRegistrationHelper*>					WidgetClassMap;
typedef std::map<String, WidgetRegistrationHelper*>::iterator		WidgetClassMapIterator;
typedef std::pair<String, WidgetRegistrationHelper*>				WidgetClassMapPair;