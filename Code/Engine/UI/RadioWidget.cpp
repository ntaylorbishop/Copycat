#include "Engine/UI/RadioWidget.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/EventSystem/GlobalEventSystem.hpp"
#include "Engine/UI/Widgets/WidgetRegistrationHelper.hpp"
#include "Engine/UI/WidgetPropertyAnim.hpp"
#include <algorithm>
#include "Engine/UI/UISystem.hpp"

STATIC WidgetRegistrationHelper RadioWidget::s_radioWidgetRegistrationHelper("Radio", &RadioWidget::CreateRadioWidget, &RadioWidget::CreateRadioWidgetFromXML);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void RadioWidget::InitializeWidgetSpecificDefaultPropertiesAndEvents() {

	WidgetPropertiesMapIterator widgetIt = m_codeDefaultWidgetProperties.find(WIDGET_STATE_DEFAULT);
	NamedProperties* defaultProperties = widgetIt->second;

	defaultProperties->SetProperty("radioGrouping", RADIO_GROUPING_TOP_TO_BOTTOM);
	defaultProperties->SetProperty("radioTextAlign", RADIO_TEXT_ALIGN_LEFT);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void RadioWidget::Update(float deltaSeconds) {
	UpdateOrderID();
	WidgetBase::Update(deltaSeconds);
}

VIRTUAL void RadioWidget::DoOnClickStuff() {

	for (uint i = 0; i < m_parent->m_widgets.size(); i++) {
		WidgetBase* otherWidget = m_parent->m_widgets[i];

		String myName;
		String otherName;
		GetProperty("name", myName);
		otherWidget->GetProperty("name", otherName);

		if (otherWidget->GetWidgetType() == WIDGET_TYPE_RADIO && myName == otherName && otherWidget != this) {

			otherWidget->SetWidgetState(WIDGET_STATE_ENABLED);
		}
	}
}



//---------------------------------------------------------------------------------------------------------------------------
void RadioWidget::Render() const {
	WidgetBase::Render();

	IntVector2 adjustedPosInGrouping = GetResolvedPosition();
	IntVector2 size = GetResolvedSize();

	int padding;
	GetProperty<int>("padding", padding);

	adjustedPosInGrouping.y -= (padding + size.y) * m_orderID;

	RenderRadioButton();
	RenderText();
}


//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL AABB2 RadioWidget::GetContainingQuad() const {

	IntVector2 pos = GetResolvedPosition();
	IntVector2 buttonSize = GetResolvedSize();

	Font* font;
	String text;
	float fontSize;
	int paddingLeft;
	GetProperty("fontType", font);
	GetProperty("text", text);
	GetProperty("fontSize", fontSize);
	GetProperty("padding-left", paddingLeft);

	float textWidth = font->GetWidthOfString(text, fontSize);
	float lineHeight = font->GetLineHeight() * fontSize;

	IntVector2 quadSize;
	quadSize.x = (int)textWidth + paddingLeft + buttonSize.x;
	quadSize.y = (int)std::max(lineHeight, (float)buttonSize.y);

	return AABB2(pos.ToVector2(), pos.ToVector2() + quadSize.ToVector2());
}


//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL IntVector2 RadioWidget::GetResolvedPosition() const {

	IntVector2 resolvedPos = WidgetBase::GetResolvedPosition();
	IntVector2 resolvedSize = WidgetBase::GetResolvedSize();

	int padding;
	GetProperty("padding", padding);

	resolvedPos.y -= m_orderID * resolvedSize.y - padding;

	return resolvedPos;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetBase* RadioWidget::CreateRadioWidget(const IntVector2& position) {
	RadioWidget* newRadioWidget = new RadioWidget();
	newRadioWidget->Initialize(position);
	return newRadioWidget;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetBase* RadioWidget::CreateRadioWidgetFromXML(const XMLNode& widgetData) {
	RadioWidget* newRadioWidget = new RadioWidget();
	newRadioWidget->Initialize(widgetData);
	return newRadioWidget;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET-SPECIFIC UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IntVector2 RadioWidget::GetSizeWithText() {

	IntVector2 size = GetResolvedSize();
	int xTextOffset;
	String text;
	Font* fontType;
	float fontSize;
	GetProperty("padding-left", xTextOffset);
	GetProperty("text", text);
	GetProperty("fontType", fontType);
	GetProperty("fontSize", fontSize);

	float textWidth = fontType->GetWidthOfString(text, fontSize);

	return IntVector2((int)(textWidth + (float)xTextOffset + (float)size.x), (int)std::max((float)size.y, fontType->GetLineHeight() * fontSize));
}

//---------------------------------------------------------------------------------------------------------------------------
void RadioWidget::UpdateOrderID() {

	String name;
	GetProperty("name", name);

	m_orderID = 0;
	for (uint i = 0; i < m_parent->m_widgets.size(); i++) {

		RadioWidget* otherRadioWidget = dynamic_cast<RadioWidget*>(m_parent->m_widgets[i]);

		if (!otherRadioWidget) {
			continue;
		}

		String otherName;
		otherRadioWidget->GetProperty("name", otherName);
		if (name == otherName && this != otherRadioWidget) {
			m_orderID++;
		}
		else if (this == otherRadioWidget) {
			break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WIDGET-SPECIFIC RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void RadioWidget::RenderRadioButton() const {

	Texture* radioButtonBG;
	GetProperty("backgroundImg", radioButtonBG);

	IntVector2 size = GetResolvedSize();
	IntVector2 pos = GetResolvedPosition();
	AABB2 quad = AABB2(pos.ToVector2(), (pos + size).ToVector2());

	BeirusRenderer::DrawTexturedAABB2(radioButtonBG, RGBA::WHITE, quad);
}


//---------------------------------------------------------------------------------------------------------------------------
void RadioWidget::RenderText() const {

	String textToRender;
	GetProperty("text", textToRender);
	
	IntVector2 btnSize = GetResolvedSize();
	IntVector2 btnPos = GetResolvedPosition();

	int paddingLeft;
	Font* font;
	float fontSize;
	
	WidgetPropertyAnim<RGBA>* fontColorAnim;
	bool gotProperty = GetProperty("fontColorAnim", fontColorAnim);
	RGBA fontColor;
	if (gotProperty) {
		fontColor = fontColorAnim->Evaluate();
	}
	else {
		GetProperty("fontColor", fontColor);
	}


	GetProperty("padding-left", paddingLeft);
	GetProperty("fontType", font);
	GetProperty("fontSize", fontSize);

	float lineHeight = font->GetHeightOfString(textToRender, fontSize);
	float baseComp = font->GetLineHeight() - font->GetLineBase();
	baseComp *= fontSize;

	
	Vector2 textOffset = btnPos.ToVector2() + Vector2(btnSize.x + (float)paddingLeft, abs(btnSize.y - lineHeight) / 2.f + baseComp / 2.f);
	font->DrawText2D(textOffset, textToRender, fontSize, fontColor);
}