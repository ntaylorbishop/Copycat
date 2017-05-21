#include "Engine/UI/ButtonWidget.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/UI/Widgets/WidgetRegistrationHelper.hpp"
#include "Engine/UI/WidgetPropertyAnim.hpp"


STATIC WidgetRegistrationHelper ButtonWidget::s_buttonWidgetRegistration("Button", &ButtonWidget::CreateWidget, &ButtonWidget::CreateWidget);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetBase* ButtonWidget::CreateWidget(const IntVector2& position) {
	ButtonWidget* newButtonWidget = new ButtonWidget();
	newButtonWidget->Initialize(position);
	return newButtonWidget;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC WidgetBase* ButtonWidget::CreateWidget(const XMLNode& widgetData) {
	ButtonWidget* newButtonWidget = new ButtonWidget();
	newButtonWidget->Initialize(widgetData);
	return newButtonWidget;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ButtonWidget::Update(float deltaSeconds) {
	WidgetBase::Update(deltaSeconds);

/*
	IntVector2 resolvedPos = WidgetBase::GetResolvedPosition();
	IntVector2 resolvedSize = WidgetBase::GetResolvedSize();

	AABB2 quadToDraw = AABB2(resolvedPos.ToVector2(), resolvedPos.ToVector2() + resolvedSize.ToVector2());

	bool mouseIsInQuad = InputSystem::IsMouseWithinQuad(quadToDraw);
	bool mouseDown = InputSystem::GetMouseBtn(MOUSE_LEFT);

	if (mouseIsInQuad && mouseDown) {
		m_currentState = WIDGET_STATE_PRESSED;
	}
	else if (mouseIsInQuad && !mouseDown) {
		m_currentState = WIDGET_STATE_HOVER;
	}
	else if (!mouseIsInQuad && !mouseDown) {
		m_currentState = WIDGET_STATE_ENABLED;
	}*/
}



//---------------------------------------------------------------------------------------------------------------------------
void ButtonWidget::Render() const {
	WidgetBase::Render();

	RenderBackground();
	RenderText();
}

//---------------------------------------------------------------------------------------------------------------------------
void ButtonWidget::RenderBackground() const {

	Texture* bgImg;
	GetProperty("backgroundImg", bgImg);
	IntVector2 resolvedPos = GetResolvedPosition();
	IntVector2 resolvedSize = GetResolvedSize();

	AABB2 quadToDraw = AABB2(resolvedPos.ToVector2(), resolvedPos.ToVector2() + resolvedSize.ToVector2());

	BeirusRenderer::DrawTexturedAABB2(bgImg, RGBA::WHITE, quadToDraw);
}


//---------------------------------------------------------------------------------------------------------------------------
void ButtonWidget::RenderText() const {

	WidgetPropertyAnim<RGBA>* fontColorAnim;
	RGBA fontColor;
	bool gotProperty = GetProperty("fontColor", fontColorAnim);
	if (gotProperty) {
		fontColor = fontColorAnim->Evaluate();
	}
	else {
		GetProperty("fontColor", fontColor);
	}
	
	Font* font;
	float fontSize;
	String text;
	GetProperty("fontSize", fontSize);
	GetProperty("text", text);
	GetProperty("fontType", font);

	IntVector2 resolvedPos = GetResolvedPosition();
	IntVector2 resolvedSize = GetResolvedSize();

	float stringLengthSize = font->GetWidthOfString(text, fontSize);
	float stringLineHeight = font->GetLineHeight() * fontSize;

	float baseComp = font->GetLineHeight() - font->GetLineBase();
	baseComp *= fontSize;

	float offsetX = (resolvedSize.x - stringLengthSize) / 2.f;
	float offsetY = abs(resolvedSize.y - stringLineHeight) / 2.f + baseComp / 2.f;
	Vector2 textPosInBtn = Vector2(resolvedPos.x + offsetX, resolvedPos.y + offsetY);

	font->DrawText2D(textPosInBtn, text, fontSize, fontColor);
}