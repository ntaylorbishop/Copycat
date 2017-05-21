#include "Engine/UI/WidgetBase.hpp"
#include "Engine/UI/UISystem.hpp"
#include "Engine/UI/WidgetGroup.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/Fonts/Font.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/UI/Utils/UISystemEnums.hpp"
#include "Engine/UI/RadioWidget.hpp"
#include "Engine/UI/Widgets/WidgetRegistrationHelper.hpp"
#include "Engine/EventSystem/GlobalEventSystem.hpp"

STATIC size_t WidgetBase::s_widgetID = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC CREATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetBase* WidgetBase::CreateWidgetByName(const String& name, const IntVector2& position) {

	ASSERT_OR_DIE(WidgetRegistrationHelper::s_widgetClassRegistrationMap != nullptr, "WIDGET REGISTRATION MAP IS NULL");

	WidgetClassMapIterator findNameIt = WidgetRegistrationHelper::s_widgetClassRegistrationMap->find(name);
	ASSERT_OR_DIE(findNameIt != WidgetRegistrationHelper::s_widgetClassRegistrationMap->end(), "ERROR: WIDGET TYPE DOES NOT EXIST");

	WidgetRegistrationHelper* widgetRegistrationHelper = findNameIt->second;
	WidgetBase* newWidget = (*widgetRegistrationHelper->m_widgetCreationFunc)(position);

	return newWidget;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetBase* WidgetBase::CreateWidgetByName(const String& name, const XMLNode& widgetData) {

	ASSERT_OR_DIE(WidgetRegistrationHelper::s_widgetClassRegistrationMap != nullptr, "WIDGET REGISTRATION MAP IS NULL");

	WidgetClassMapIterator findNameIt = WidgetRegistrationHelper::s_widgetClassRegistrationMap->find(name);
	ASSERT_OR_DIE(findNameIt != WidgetRegistrationHelper::s_widgetClassRegistrationMap->end(), "ERROR: WIDGET TYPE DOES NOT EXIST");

	WidgetRegistrationHelper* widgetRegistrationHelper = findNameIt->second;
	WidgetBase* newWidget = (*widgetRegistrationHelper->m_widgetCreationFromXMLFunc)(widgetData);

	return newWidget;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void WidgetBase::Update(float deltaSeconds) { 

	UNUSED(deltaSeconds);
	bool isBeingHoveredOn = CheckIfHovered();

	if (isBeingHoveredOn) {
		UISystem::WidgetConsumeInput();

		if (m_currentState == WIDGET_STATE_ENABLED) {
			m_currentState = WIDGET_STATE_HOVER;
			FireAssociatedEvent("onHover");
		}
		else if (m_currentState == WIDGET_STATE_ENABLED_SELECTED) {
			m_currentState = WIDGET_STATE_HOVER_SELECTED;
			FireAssociatedEvent("onHover");
		}
	}

	bool isBeingPressed = CheckIfPressed();

	if (isBeingPressed) {
		UISystem::WidgetConsumeInput();

		if (m_currentState == WIDGET_STATE_HOVER) {
			m_currentState = WIDGET_STATE_PRESSED;
			FireAssociatedEvent("onPressed");
		}
		else if (m_currentState == WIDGET_STATE_HOVER_SELECTED) {
			m_currentState = WIDGET_STATE_PRESSED_SELECTED;
			FireAssociatedEvent("onPressed");
		}
	}

	if (!isBeingHoveredOn) {
		if (m_currentState == WIDGET_STATE_HOVER_SELECTED || m_currentState == WIDGET_STATE_PRESSED_SELECTED) {
			m_currentState = WIDGET_STATE_ENABLED_SELECTED;
		}
		else if (m_currentState == WIDGET_STATE_HOVER || m_currentState == WIDGET_STATE_PRESSED) {
			m_currentState = WIDGET_STATE_ENABLED;
		}
	}

	bool hasBeenClicked = CheckIfClicked();

	if (hasBeenClicked) {
		if (m_currentState == WIDGET_STATE_PRESSED) {
			m_currentState = WIDGET_STATE_ENABLED_SELECTED;
			FireAssociatedEvent("onClick");
		}
		else if (m_currentState == WIDGET_STATE_PRESSED_SELECTED) {
			m_currentState = WIDGET_STATE_HOVER_SELECTED;
			FireAssociatedEvent("onClick");
		}

		DoOnClickStuff();
	}



	//UPDATE FONT COLOR
	WidgetPropertyAnim<RGBA>* fontColorAnim = UISystem::GetWidgetAnimationByName<RGBA>("fontColor");
	if (fontColorAnim) {
		fontColorAnim->Update(deltaSeconds);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void WidgetBase::Render() const { 

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PROPERTIES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void WidgetBase::SetPropertyFromXML(NamedProperties* statePropertiesToAddTo, const XMLAttribute& attrib, eWidgetState currStateNode) {

	String attribName = attrib.lpszName;
	String attribValue = attrib.lpszValue;

	eWidgetPropertyType propertyType = WidgetProperty::GetPropertyTypeFromPropertyName(attribName);

	switch (propertyType) {
	//REGULAR PROPERTIES
	case WIDGET_PROPERTY_TYPE_FLOAT: {
		float propertyValue = XMLUtils::ParseFloat(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_COLOR: {
		RGBA propertyValue = XMLUtils::ParseHexColor(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_STRING: {
		String propertyValue = attribValue;
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_INTVECTOR2: {
		IntVector2 propertyValue = XMLUtils::ParseIntVector2(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_VECTOR2: {
		Vector2 propertyValue = XMLUtils::ParseVector2(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_INT: {
		int propertyValue = XMLUtils::ParseInt(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_UINT: {
		uint propertyValue = (uint)XMLUtils::ParseInt(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, (uint)propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_TEXTURE: {
		Texture* propertyValue = Texture::CreateOrGetTexture(attribValue);
		ASSERT_OR_DIE(propertyValue != nullptr, "ERROR: Texture does not exist trying to parse UI XML.");
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_FONT: {
		Font* propertyValue = Font::CreateOrGetFont(attribValue);
		ASSERT_OR_DIE(propertyValue != nullptr, "ERROR: That font does not exist.");
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_RADIOGROUPING: {
		eRadioGrouping propertyValue = RadioWidget::GetRadioGroupingEnumFromString(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_RADIOTEXTALIGN: {
		eRadioTextAlign propertyValue = RadioWidget::GetRadioTextAlignEnumFromString(attribValue);
		statePropertiesToAddTo->SetProperty(attribName, propertyValue);
		return;
	}
	case WIDGET_PROPERTY_TYPE_SKIN: {
		return;
	}
	//EVENTS
	case WIDGET_PROPERTY_EVENT_ONCLICK: {
		String eventName = attribValue;
		statePropertiesToAddTo->SetProperty(attribName, eventName);
		return;
	}
	case WIDGET_PROPERTY_EVENT_ONHOVER: {
		if (currStateNode != WIDGET_STATE_HOVER || currStateNode != WIDGET_STATE_HOVER_SELECTED) {
			return;
		}
		String eventName = attribValue;
		statePropertiesToAddTo->SetProperty(attribName, eventName);
		return;
	}
	case WIDGET_PROPERTY_EVENT_ONVALUE: {
		String eventName = attribValue;
		statePropertiesToAddTo->SetProperty(attribName, eventName);
		return;
	}
	case WIDGET_PROPERTY_EVENT_ONPRESSED: {
		if (currStateNode != WIDGET_STATE_PRESSED || currStateNode != WIDGET_STATE_PRESSED_SELECTED) {
			return;
		}
		String eventName = attribValue;
		statePropertiesToAddTo->SetProperty(attribName, eventName);
		return;
	}
	//ANIMS
	case WIDGET_PROPERTY_ANIM_ONHOVER: {
		String animName = attribValue;
		UISystem::TryGetAnimatedProperty(statePropertiesToAddTo, animName);
		return;
	}
	case WIDGET_PROPERTY_ANIM_ONCLICK: {
		String animName = attribValue;
		UISystem::TryGetAnimatedProperty(statePropertiesToAddTo, animName);
		return;
	}
	case WIDGET_PROPERTY_ANIM_ONVALUE: {
		String animName = attribValue;
		UISystem::TryGetAnimatedProperty(statePropertiesToAddTo, animName);
	}
	case WIDGET_PROPERTY_ANIM_ONPRESSED: {
		String animName = attribValue;
		UISystem::TryGetAnimatedProperty(statePropertiesToAddTo, animName);
		return;
	}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
IntVector2 WidgetBase::GetResolvedPosition() const {

	//TODO: RESOLVE BASED ON ANCHORING, DOCKING, ETC.
	IntVector2 absPos;
	GetProperty<IntVector2>("absolutePosition", absPos);
	return absPos;
}


//---------------------------------------------------------------------------------------------------------------------------
IntVector2	WidgetBase::GetResolvedSize() const {

	//TODO: RESOLVE BASED ON MAX WIDTH, HEIGHT, ETC.
	//WARNING: MIGHT BE INT
	uint width;
	uint height;
	GetProperty("width", width);
	GetProperty("height", height);

	return IntVector2(width, height);
}


//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL AABB2 WidgetBase::GetContainingQuad() const {

	IntVector2 size = GetResolvedSize();
	IntVector2 pos = GetResolvedPosition();

	return AABB2(pos.ToVector2(), pos.ToVector2() + size.ToVector2());
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
WidgetBase::WidgetBase(WidgetGroup* parent, eWidgetType widgetType, eWidgetState widgetState)
	: m_parent(parent)
	, m_widgetType(widgetType)
	, m_currentState(widgetState)
	, m_skin(UISystem::GetSkin("BeirusSkin"))
	, m_name("NULL")
	, m_widgetID(s_widgetID++)
{ }


//---------------------------------------------------------------------------------------------------------------------------
WidgetBase::~WidgetBase() {

	WidgetPropertiesMapIterator propertiesIt = m_widgetProperties.begin();

	for (propertiesIt; propertiesIt != m_widgetProperties.end(); ++propertiesIt) {
		delete propertiesIt->second;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INITIALIZATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void WidgetBase::Initialize(const IntVector2& position) {
	InitializeDefaultProperties();
	InitializeWidgetSpecificDefaultPropertiesAndEvents();

	NamedProperties* defaultProperties = GetWidgetPropertiesForState(WIDGET_STATE_DEFAULT);
	if (defaultProperties) {
		defaultProperties->SetProperty("absolutePosition", position);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void WidgetBase::Initialize(const XMLNode& widgetData) {
	InitializeDefaultProperties();
	InitializeWidgetSpecificDefaultPropertiesAndEvents();
	InitializeFromXML(widgetData);
}


//---------------------------------------------------------------------------------------------------------------------------
void WidgetBase::InitializeFromXML(const XMLNode& widgetNode) {

	NamedProperties* defaultProperties = new NamedProperties();
	m_widgetProperties.insert(WidgetPropertiesMapPair(WIDGET_STATE_DEFAULT, defaultProperties));

	//BASIC ATTRIBS
	for (int attribIdx = 0; attribIdx < widgetNode.nAttribute(); attribIdx++) {

		XMLAttribute currAttrib = widgetNode.getAttribute(attribIdx);

		if (strcmp(currAttrib.lpszName, g_keyInData_Name) == 0) {
			m_name = currAttrib.lpszValue;
		}
		else if (strcmp(currAttrib.lpszName, g_keyInData_Skin) == 0) {
			m_skin = UISystem::GetSkin(currAttrib.lpszValue);
		}

		SetPropertyFromXML(defaultProperties, currAttrib, WIDGET_STATE_DEFAULT);
	}

	//STATE PROPERTIES
	for (int stateIdx = 0; stateIdx < widgetNode.nChildNode(); stateIdx++) {

		XMLNode statePropertiesNode = widgetNode.getChildNode(stateIdx);

		eWidgetState currStateNode = GetWidgetStateFromString(statePropertiesNode.getName());

		NamedProperties* stateWidgetProperties;

		if (currStateNode != WIDGET_STATE_DEFAULT) {
			stateWidgetProperties = new NamedProperties();
			m_widgetProperties.insert(WidgetPropertiesMapPair(currStateNode, stateWidgetProperties));
		}
		else {
			stateWidgetProperties = defaultProperties;
		}

		for (int propertyIdx = 0; propertyIdx < statePropertiesNode.nAttribute(); propertyIdx++) {
			SetPropertyFromXML(stateWidgetProperties, statePropertiesNode.getAttribute(propertyIdx), currStateNode);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void WidgetBase::InitializeDefaultProperties() {

	//TODO: I want to be able to have specific widgets implement their own version of setting default properties

	//ALL-WIDGETS C++
	NamedProperties* defaultProperties = new NamedProperties();
	m_codeDefaultWidgetProperties.insert(WidgetPropertiesMapPair(WIDGET_STATE_DEFAULT, defaultProperties));

	defaultProperties->SetProperty("fontType",			Font::CreateOrGetFont("Tahoma"));
	defaultProperties->SetProperty("fontColor",			RGBA::WHITE);
	defaultProperties->SetProperty("fontSize",			0.4f);
	defaultProperties->SetProperty("width",				(uint)100);
	defaultProperties->SetProperty("height",			(uint)100);
	defaultProperties->SetProperty("backgroundColor",	RGBA(0.f, 0.f, 0.f, 0.5f));
	defaultProperties->SetProperty("borderColor",		RGBA::WHITE);
	defaultProperties->SetProperty("backgroundImg",		nullptr);
	defaultProperties->SetProperty("absolutePosition",	IntVector2(100, 100));
	defaultProperties->SetProperty("relativePosition",	IntVector2(0, 0));
	defaultProperties->SetProperty("text",				String("default"));
	defaultProperties->SetProperty("backgroundImgOn",	nullptr);
	defaultProperties->SetProperty("backgroundImgOff",	nullptr);
	defaultProperties->SetProperty("padding",			(int)0);
	defaultProperties->SetProperty("padding-left",		(int)0);
	defaultProperties->SetProperty("padding-right",		(int)0);
	defaultProperties->SetProperty("padding-top",		(int)0);
	defaultProperties->SetProperty("padding-bottom",	(int)0);
	defaultProperties->SetProperty("onClick",			String("NULL"));
	defaultProperties->SetProperty("onHover",			String("NULL"));
	defaultProperties->SetProperty("onPressed",			String("NULL"));
	defaultProperties->SetProperty("onValue",			String("NULL"));

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE: STATE SWITCHING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
bool WidgetBase::CheckIfHovered() {

	AABB2 containingQuad = GetContainingQuad();
	return InputSystem::IsMouseWithinQuad(containingQuad);
}


//---------------------------------------------------------------------------------------------------------------------------
bool WidgetBase::CheckIfPressed() {

	AABB2 containingQuad = GetContainingQuad();
	return InputSystem::IsMouseWithinQuad(containingQuad) && InputSystem::GetMouseBtn(MOUSE_LEFT);
}


//---------------------------------------------------------------------------------------------------------------------------
bool WidgetBase::CheckIfClicked() {

	AABB2 containingQuad = GetContainingQuad();
	return InputSystem::IsMouseWithinQuad(containingQuad) && InputSystem::GetMouseBtnUp(MOUSE_LEFT);
}


//---------------------------------------------------------------------------------------------------------------------------
void WidgetBase::FireAssociatedEvent(const String& eventName) {

	String onPressedEventName;
	GetProperty<String>(eventName, onPressedEventName);

	if (onPressedEventName != g_keyInData_NoEventAssociated) {
		EventSystem::FireEvent(onPressedEventName);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PROTECTED GETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
NamedProperties* WidgetBase::GetWidgetPropertiesForState(eWidgetState widgetState) {

	WidgetPropertiesMapConstIterator propertiesIt = m_widgetProperties.find(widgetState);

	return (propertiesIt != m_widgetProperties.end())
		? (propertiesIt->second)
		: (nullptr);
}