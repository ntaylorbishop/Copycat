#include "Engine/UI/LabelWidget.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void LabelWidget::Update(float deltaSeconds) {
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
void LabelWidget::Render() const {
	WidgetBase::Render();

/*
	RenderBackground();
	RenderText();*/
}

