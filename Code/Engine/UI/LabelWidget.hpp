#pragma once

#include "Engine/UI/WidgetBase.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

class LabelWidget : public WidgetBase {
public:
	//STRUCTORS INIT
	LabelWidget::LabelWidget()									: WidgetBase(nullptr, WIDGET_TYPE_LABEL, WIDGET_STATE_ENABLED) { }
	LabelWidget::LabelWidget(WidgetGroup* belongingWidgetGroup) : WidgetBase(belongingWidgetGroup, WIDGET_TYPE_LABEL, WIDGET_STATE_ENABLED) { }
	LabelWidget::~LabelWidget() { }



	//UPDATE RENDER
	void LabelWidget::Update(float deltaSeconds);
	void LabelWidget::Render() const;

private:

};