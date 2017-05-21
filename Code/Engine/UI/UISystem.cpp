#include "Engine/UI/UISystem.hpp"
#include "Engine/UI/WidgetBase.hpp"
#include "Engine/UI/ButtonWidget.hpp"
#include "Engine/UI/WidgetSkin.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Utils/HardwareUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/UI/Widgets/WidgetRegistrationHelper.hpp"
#include "Engine/UI/ButtonWidget.hpp"
#include "Engine/UI/RadioWidget.hpp"
#include "Engine/UI/WidgetPropertyAnim.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/UI/Utils/WidgetProperty.hpp"

STATIC UISystem* UISystem::s_theUISystem = nullptr;

#ifdef USING_UI_SYSTEM
static void COMPILER_IS_STUPID_SO_HELP_IT_COMPILE_IN() {

	ButtonWidget* buttonWidget = new ButtonWidget();
	delete buttonWidget;
	RadioWidget* radioWidget = new RadioWidget();
	delete radioWidget;
}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void UISystem::Initialize() {

	#ifdef USING_UI_SYSTEM
	ASSERT_OR_DIE(s_theUISystem == nullptr, "ERROR: UISystem already initialized.");
	s_theUISystem = new UISystem();
	LoadInAllXMLData("Data/UI/");

	COMPILER_IS_STUPID_SO_HELP_IT_COMPILE_IN();

	Console::RegisterCommand("ui_reload", "Reloads all XML data for the UI system and clears all code-created widgets.", ReloadUI);
	Console::RegisterCommand("ui_hide", "Hides all UI elements.", HideUI);
	Console::RegisterCommand("ui_show", "Shows all UI elements.", ShowUI);
	#endif
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void UISystem::Shutdown() {

	#ifdef USING_UI_SYSTEM
	ASSERT_OR_DIE(s_theUISystem != nullptr, "ERROR: UISystem already destroyed or has not been initialized.");
	delete s_theUISystem;
	s_theUISystem = nullptr;
	#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void UISystem::Update(float deltaSeconds) {

	#ifdef USING_UI_SYSTEM

	if (InputSystem::GetKeyDown(g_uiSystemReloadKey)) {
		Shutdown();
		Initialize();
	}
	if (InputSystem::GetKeyDown(g_uiSystemToggleKey)) {
		ToggleVisibility();
	}

	s_theUISystem->m_mouseInputConsumed = false;

	WidgetVectorReverseIterator widgetIt = s_theUISystem->m_widgets.rbegin();
	for (widgetIt; widgetIt != s_theUISystem->m_widgets.rend(); ++widgetIt) {
		(*widgetIt)->Update(deltaSeconds);
	}

	WidgetGroupVectorReverseIterator groupIt = s_theUISystem->m_widgetGroups.rbegin();
	for (groupIt; groupIt != s_theUISystem->m_widgetGroups.rend(); ++groupIt) {
		(*groupIt)->Update(deltaSeconds);
	}

	#else
	UNUSED(deltaSeconds);
	#endif
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void UISystem::Render() {

	#ifdef USING_UI_SYSTEM

	if (s_theUISystem->m_isHidden) {
		return;
	}

	for (uint i = 0; i < s_theUISystem->m_widgetGroups.size(); i++) {
		s_theUISystem->m_widgetGroups[i]->Render();
	}

	for (uint i = 0; i < s_theUISystem->m_widgets.size(); i++) {
		s_theUISystem->m_widgets[i]->Render();
	}

	#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SKINS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void UISystem::AddSkin(WidgetSkin* newSkin) {

	size_t nameHash = std::hash<String>()(newSkin->GetName());
	SkinMapIterator skinIt = s_theUISystem->m_skins.find(nameHash);

	ASSERT_OR_DIE(skinIt == s_theUISystem->m_skins.end(), "ERROR: Skin with that name already exists.");

	s_theUISystem->m_skins.insert(SkinMapPair(nameHash, newSkin));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetSkin* UISystem::GetSkin(const String& skinName) {

	size_t nameHash = std::hash<String>()(skinName);
	SkinMapIterator skinIt = s_theUISystem->m_skins.find(nameHash);

	ASSERT_OR_DIE(skinIt != s_theUISystem->m_skins.end(), "ERROR: Skin with that name already exists.");

	return skinIt->second;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void UISystem::TryGetAnimatedProperty(NamedProperties* propertiesToAddTo, const String& animName) {

	//TRY GET FLOAT PROPERTY
	WidgetPropertyAnim<float>* animatedPropertyFloat = UISystem::GetWidgetAnimationByName<float>(animName);
	if (animatedPropertyFloat != nullptr) {
		propertiesToAddTo->SetProperty(animatedPropertyFloat->m_propertyName + "Anim", animatedPropertyFloat);
		return;
	}

	//TRY GET RGBA PROPERTY
	WidgetPropertyAnim<RGBA>* animatedPropertyRGBA = UISystem::GetWidgetAnimationByName<RGBA>(animName);
	if (animatedPropertyRGBA != nullptr) {
		propertiesToAddTo->SetProperty(animatedPropertyRGBA->m_propertyName + "Anim", animatedPropertyRGBA);
		return;
	}

	//TRY GET INTVECTOR2 PROPERTY
	WidgetPropertyAnim<IntVector2>* animatedPropertyIntVector2 = UISystem::GetWidgetAnimationByName<IntVector2>(animName);
	if (animatedPropertyIntVector2 != nullptr) {
		propertiesToAddTo->SetProperty(animatedPropertyIntVector2->m_propertyName + "Anim", animatedPropertyIntVector2);
		return;
	}

	//TRY GET INT PROPERTY
	WidgetPropertyAnim<int>* animatedPropertyInt = UISystem::GetWidgetAnimationByName<int>(animName);
	if (animatedPropertyInt != nullptr) {
		propertiesToAddTo->SetProperty(animatedPropertyInt->m_propertyName + "Anim", animatedPropertyInt);
		return;
	}

	ERROR_AND_DIE("ERROR: That animated property does not exist.");
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOADING IN FROM XML
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParseSkins(const XMLNode& skinsNode);
void ParseFile(const String& filename);
void ParseWidgetAnimations(const XMLNode& animsNode);
void ParseWidgetAnimation(const XMLNode& animNode);

//---------------------------------------------------------------------------------------------------------------------------
void ParseSkins(const XMLNode& skinsNode) {

	for (int i = 0; i < skinsNode.nChildNode(); i++) {

		XMLNode skinNode = skinsNode.getChildNode(i);
		ASSERT_OR_DIE(strcmp(skinNode.getName(), g_keyInData_SkinSpecifier) == 0, "ERROR: Not a valid UI Skin defined.");
		WidgetSkin::CreateSkin(skinNode);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ParseUIWidgets(const XMLNode& uiWidgetsNode) {

	for (int i = 0; i < uiWidgetsNode.nChildNode(); i++) {

		XMLNode widgetOrWidgetGroupNode = uiWidgetsNode.getChildNode(i);

		if (strcmp(widgetOrWidgetGroupNode.getName(), g_keyInData_UIWidget) == 0) {
			String widgetName = widgetOrWidgetGroupNode.getAttribute(g_keyInData_Type);
			WidgetBase* newWidget = WidgetBase::CreateWidgetByName(widgetName, widgetOrWidgetGroupNode);
			UISystem::AddWidget(newWidget);
		}
		else if (strcmp(widgetOrWidgetGroupNode.getName(), g_keyInData_WidgetGroup) == 0) {
			WidgetGroup::CreateWidgetGroupFromXML(widgetOrWidgetGroupNode, nullptr);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ParseWidgetAnimations(const XMLNode& animsNode) {

	for (int i = 0; i < animsNode.nChildNode(); i++) {
		
		XMLNode animProperty = animsNode.getChildNode(i);

		if (strcmp(animProperty.getName(), g_keyInData_PropertyAnim) == 0) {
			ParseWidgetAnimation(animProperty);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET ANIMATIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
String GetPropertyNameInWidgetAnimation(const XMLNode& animNode) {

	XMLNode firstKeyframe = animNode.getChildNode(0);
	XMLAttribute propertyAttrib = firstKeyframe.getAttribute(1);
	String propertyTypeName = propertyAttrib.lpszName;
	return propertyTypeName;
}

//---------------------------------------------------------------------------------------------------------------------------
eWidgetPropertyType GetPropertyTypeOfWidgetAnimation(const XMLNode& firstKeyFrame) {

	XMLAttribute propertyAttrib = firstKeyFrame.getAttribute(1);
	String propertyTypeName = propertyAttrib.lpszName;
	String propertyTypeValue = propertyAttrib.lpszValue;
	eWidgetPropertyType propertyType = WidgetProperty::GetPropertyTypeFromPropertyName(propertyTypeName);
	return propertyType;
}


//---------------------------------------------------------------------------------------------------------------------------
float GetDurationOfWidgetAnimatedProperty(const XMLNode& animProperty) {

	float duration = 0;
	for (int i = 0; i < animProperty.nChildNode(); i++) {

		XMLNode keyframe = animProperty.getChildNode(i);
		float keyframeTime = XMLUtils::ParseFloat(keyframe.getAttribute(0).lpszValue);
		if (keyframeTime > duration) {
			duration = keyframeTime;
		}
	}
	return duration;
}


//---------------------------------------------------------------------------------------------------------------------------
void ParseWidgetAnimation(const XMLNode& animNode) {

	if (strcmp(animNode.getName(), g_keyInData_PropertyAnim) != 0) {
		return;
	}

	ASSERT_OR_DIE(strcmp(animNode.getAttribute(0).lpszName, g_keyInData_animPropertyName) == 0, "ERROR: AnimatedProperty must have name attribute");
	ASSERT_OR_DIE(animNode.nChildNode() > 0, "ERROR: AnimatedProperty must have at least 1 keyframe");

	eWidgetPropertyType propertyType = GetPropertyTypeOfWidgetAnimation(animNode.getChildNode(0));
	float durationOfAnim = GetDurationOfWidgetAnimatedProperty(animNode);
	eAnimPropertyMode animMode = GetAnimPropertyModeFromString(animNode.getAttribute(1).lpszValue);
	String propertyName = GetPropertyNameInWidgetAnimation(animNode);
	String animName = animNode.getAttribute(0).lpszName;

	switch (propertyType) {
	case WIDGET_PROPERTY_TYPE_FLOAT: {
		WidgetPropertyAnim<float>* newAnim = new WidgetPropertyAnim<float>(durationOfAnim, animMode, propertyName, animName);

		for (int i = 0; i < animNode.nChildNode(); i++) {

			XMLNode keyframe = animNode.getChildNode(i);
			ASSERT_OR_DIE(strcmp(keyframe.getName(), g_keyInData_keyframeName) == 0, "ERROR: AnimatedProperties must contain keyframes.");

			String keyframePropertyTime = keyframe.getAttribute(0).lpszValue;
			String keyframePropertyValue = keyframe.getAttribute(1).lpszValue;

			float keyframeTime = XMLUtils::ParseFloat(keyframePropertyTime);
			float keyframeValue = XMLUtils::ParseFloat(keyframePropertyValue);

			newAnim->m_keyframedProperty.AddKeyframe(keyframeTime / durationOfAnim, keyframeValue);
		}
		UISystem::AddWidgetAnimation(propertyName, newAnim);
	}
	case WIDGET_PROPERTY_TYPE_COLOR: {
		WidgetPropertyAnim<RGBA>* newAnim = new WidgetPropertyAnim<RGBA>(durationOfAnim, animMode, propertyName, animName);

		for (int i = 0; i < animNode.nChildNode(); i++) {

			XMLNode keyframe = animNode.getChildNode(i);
			ASSERT_OR_DIE(strcmp(keyframe.getName(), g_keyInData_keyframeName) == 0, "ERROR: AnimatedProperties must contain keyframes.");

			String keyframePropertyTime = keyframe.getAttribute(0).lpszValue;
			String keyframePropertyValue = keyframe.getAttribute(1).lpszValue;

			float keyframeTime = XMLUtils::ParseFloat(keyframePropertyTime);
			RGBA keyframeValue = XMLUtils::ParseHexColor(keyframePropertyValue);

			newAnim->m_keyframedProperty.AddKeyframe(keyframeTime / durationOfAnim, keyframeValue);
		}
		UISystem::AddWidgetAnimation(propertyName, newAnim);
	}
	case WIDGET_PROPERTY_TYPE_INTVECTOR2: {
		WidgetPropertyAnim<IntVector2>* newAnim = new WidgetPropertyAnim<IntVector2>(durationOfAnim, animMode, propertyName, animName);

		for (int i = 0; i < animNode.nChildNode(); i++) {

			XMLNode keyframe = animNode.getChildNode(i);
			ASSERT_OR_DIE(strcmp(keyframe.getName(), g_keyInData_keyframeName) == 0, "ERROR: AnimatedProperties must contain keyframes.");
			float keyframeTime = XMLUtils::ParseFloat(keyframe.getAttribute(0).lpszValue);
			IntVector2 keyframeValue = XMLUtils::ParseIntVector2(keyframe.getAttribute(1).lpszValue);
			newAnim->m_keyframedProperty.AddKeyframe(keyframeTime / durationOfAnim, keyframeValue);
		}
		UISystem::AddWidgetAnimation(propertyName, newAnim);
	}
	case WIDGET_PROPERTY_TYPE_INT: {
		WidgetPropertyAnim<int>* newAnim = new WidgetPropertyAnim<int>(durationOfAnim, animMode, propertyName, animName);

		for (int i = 0; i < animNode.nChildNode(); i++) {

			XMLNode keyframe = animNode.getChildNode(i);
			ASSERT_OR_DIE(strcmp(keyframe.getName(), g_keyInData_keyframeName) == 0, "ERROR: AnimatedProperties must contain keyframes.");
			float keyframeTime = XMLUtils::ParseFloat(keyframe.getAttribute(0).lpszValue);
			int keyframeValue = XMLUtils::ParseInt(keyframe.getAttribute(1).lpszValue);
			newAnim->m_keyframedProperty.AddKeyframe(keyframeTime / durationOfAnim, keyframeValue);
		}
		UISystem::AddWidgetAnimation(propertyName, newAnim);
	}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ParseFile(const String& filename) {

	XMLNode root = XMLNode::parseFile(filename.c_str());

	for (int i = 0; i < root.nChildNode(); i++) {
		XMLNode child = root.getChildNode(i);

		if (strcmp(child.getName(), g_keyInData_Skins) == 0) {
			ParseSkins(child);
		}
		else if (strcmp(child.getName(), g_keyInData_UIWidgets) == 0) {
			ParseUIWidgets(child);
		}
		else if (strcmp(child.getName(), g_keyInData_PropertyAnims) == 0) {
			ParseWidgetAnimations(child);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
STATIC void UISystem::LoadInAllXMLData(const String& filepath) {

	std::vector<String> filesToParse = FileUtils::EnumerateFiles(filepath, "*.xml", true);

	for (uint i = 0; i < filesToParse.size(); i++) {
		ParseFile(filesToParse[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSOLE COMMANDS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ReloadUI(Command& args) {

	UNUSED(args);
	UISystem::Shutdown();
	UISystem::Initialize();
}


//---------------------------------------------------------------------------------------------------------------------------
void HideUI(Command& args) {

	UNUSED(args);
	UISystem::Hide();
	Console::PrintOutput("All UI widgets hidden.", CONSOLE_VERIFY);
}


//---------------------------------------------------------------------------------------------------------------------------
void ShowUI(Command& args) {

	UNUSED(args);
	UISystem::Show();
	Console::PrintOutput("All UI widgets shown.", CONSOLE_VERIFY);
}