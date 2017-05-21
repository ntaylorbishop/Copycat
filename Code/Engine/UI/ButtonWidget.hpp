#pragma once

#include "Engine/UI/WidgetBase.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

class ButtonWidget : public WidgetBase {
public:
	//STRUCTORS INIT
	ButtonWidget() : WidgetBase(nullptr, WIDGET_TYPE_BUTTON, WIDGET_STATE_ENABLED) { }
	~ButtonWidget() {} 

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	static WidgetBase* CreateWidget(const IntVector2& position);
	static WidgetBase* CreateWidget(const XMLNode& widgetData);

private:

	//PRIVATE RENDER
	void RenderBackground() const;
	void RenderText() const;

	static WidgetRegistrationHelper s_buttonWidgetRegistration;
};