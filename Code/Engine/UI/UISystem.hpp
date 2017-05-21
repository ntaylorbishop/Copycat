#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/UI/WidgetGroup.hpp"
#include "Engine/UI/WidgetBase.hpp"
#include "Engine/UI/UISystemCommon.hpp"
#include "Engine/UI/WidgetPropertyAnim.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

const char g_uiSystemReloadKey = 'U';
const char g_uiSystemToggleKey = 'H';

class WidgetBase;
class WidgetSkin;

class UISystem {
public:
	//INIT
	static void Initialize();
	static void Shutdown();

	//UPDATE RENDER
	static void Update(float deltaSeconds);
	static void Render();

	//WIDGETS
	static void AddWidget(WidgetBase* widgetToAdd) { s_theUISystem->m_widgets.push_back(widgetToAdd); }
	static void AddWidgetGroup(WidgetGroup* widgetGroupToAdd) { s_theUISystem->m_widgetGroups.push_back(widgetGroupToAdd); }

	template <typename T>
	static void AddWidgetAnimation(const String& propertyAnim, WidgetPropertyAnim<T>* newPropertyAnim);

	template <typename T>
	static WidgetPropertyAnim<T>* GetWidgetAnimationByName(const String& animName);

	//SKINS
	static void AddSkin(WidgetSkin* newSkin);
	static WidgetSkin* GetSkin(const String& skinName);

	//HIDE SHOW
	static void Hide()				{ s_theUISystem->m_isHidden = true;							}
	static void Show()				{ s_theUISystem->m_isHidden = false;						}
	static void ToggleVisibility()	{ s_theUISystem->m_isHidden = !s_theUISystem->m_isHidden;	}
	static bool IsHidden()			{ return s_theUISystem->m_isHidden;							}

	//UTILS
	static void TryGetAnimatedProperty(NamedProperties* propertiesToAddTo, const String& animName);

private:
	//STRUCTORS INIT
	UISystem() : m_isHidden(false) { }
	~UISystem() { }

	static void LoadInAllXMLData(const String& filepath);

	//INPUT CONSUMING
	static bool HasMouseInputBeenConsumed() { return s_theUISystem->m_mouseInputConsumed; }
	static void WidgetConsumeInput() { s_theUISystem->m_mouseInputConsumed = true; }


	std::vector<WidgetGroup*> m_widgetGroups;
	std::vector<WidgetBase*> m_widgets;
	std::map<size_t, WidgetSkin*> m_skins; //Keyed on skin name
	NamedProperties m_widgetAnimatedProperties;


	bool m_isHidden;
	bool m_mouseInputConsumed = false;

	static UISystem* s_theUISystem;


	friend class WidgetSkin;
	friend class WidgetBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSOLE COMMANDS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ReloadUI(Command& args);
void HideUI(Command& args);
void ShowUI(Command& args);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::map<size_t, WidgetSkin*>::iterator		SkinMapIterator;
typedef std::pair<size_t, WidgetSkin*>				SkinMapPair;
typedef std::vector<WidgetBase*>::reverse_iterator	WidgetVectorReverseIterator;
typedef std::vector<WidgetGroup*>::reverse_iterator WidgetGroupVectorReverseIterator;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCTION TEMPLATES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
STATIC void UISystem::AddWidgetAnimation(const String& propertyAnim, WidgetPropertyAnim<T>* newPropertyAnim) {

	s_theUISystem->m_widgetAnimatedProperties.SetProperty(propertyAnim, newPropertyAnim);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
STATIC WidgetPropertyAnim<T>* UISystem::GetWidgetAnimationByName(const String& animName) {

	WidgetPropertyAnim<T>* propertyAnim = nullptr;

	ePropertyGetResult didGetProperty = s_theUISystem->m_widgetAnimatedProperties.GetProperty(animName, propertyAnim);

	if (didGetProperty == PGR_SUCCESS) {
		return propertyAnim;
	}
	else {
		return nullptr;
	}
}