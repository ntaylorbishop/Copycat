#pragma once

#include "Engine/UI/WidgetBase.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum eRadioGrouping {
	RADIO_GROUPING_TOP_TO_BOTTOM = 0,
	RADIO_GROUPING_BOTTOM_TO_TOP,
	RADIO_GROUPING_LEFT_TO_RIGHT,
	RADIO_GROUPING_RIGHT_TO_LEFT,
	RADIO_GROUPING_INVALID_GROUPING
};

//---------------------------------------------------------------------------------------------------------------------------
enum eRadioTextAlign {
	RADIO_TEXT_ALIGN_LEFT = 0,
	RADIO_TEXT_ALIGN_RIGHT,
	RADIO_TEXT_ALIGN_INVALID
};

//---------------------------------------------------------------------------------------------------------------------------
enum eRadioState {
	RADIO_STATE_SELECTED = 0,
	RADIO_STATE_NOT_SELECTED
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RADIOWIDGET CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class RadioWidget : public WidgetBase {
public:
	//STRUCTORS
	RadioWidget() : WidgetBase(nullptr, WIDGET_TYPE_RADIO, WIDGET_STATE_ENABLED), m_selectedState(RADIO_STATE_NOT_SELECTED) { }
	~RadioWidget() { }

	VIRTUAL void InitializeWidgetSpecificDefaultPropertiesAndEvents();

	VIRTUAL void DoOnClickStuff();


	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	VIRTUAL AABB2 GetContainingQuad() const;
	VIRTUAL IntVector2 GetResolvedPosition() const;

	static inline eRadioGrouping GetRadioGroupingEnumFromString(const String& radioGroupingStr);
	static inline eRadioTextAlign GetRadioTextAlignEnumFromString(const String& radioTextAlignStr);

private:
	//CREATION
	static WidgetBase* CreateRadioWidget(const IntVector2& position);
	static WidgetBase* CreateRadioWidgetFromXML(const XMLNode& widgetData);
	static WidgetRegistrationHelper s_radioWidgetRegistrationHelper;

	//WIDGET-SPECIFIC GETTERS
	IntVector2 GetSizeWithText();

	//WIDGET-SPECIFIC UPDATE
	void UpdateOrderID();

	void RenderRadioButton() const;
	void RenderText() const;


	eRadioState m_selectedState;
	uint16_t m_orderID = 0;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC INLINE eRadioGrouping RadioWidget::GetRadioGroupingEnumFromString(const String& radioGroupingStr) {

	if (radioGroupingStr == "topToBottom") {
		return RADIO_GROUPING_TOP_TO_BOTTOM;
	}
	else if (radioGroupingStr == "bottomToTop") {
		return RADIO_GROUPING_BOTTOM_TO_TOP;
	}
	else if (radioGroupingStr == "leftToRight") {
		return RADIO_GROUPING_LEFT_TO_RIGHT;
	}
	else if (radioGroupingStr == "rightToLeft") {
		return RADIO_GROUPING_RIGHT_TO_LEFT;
	}
	else {
		return RADIO_GROUPING_INVALID_GROUPING;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC INLINE eRadioTextAlign RadioWidget::GetRadioTextAlignEnumFromString(const String& radioTextAlignStr) {

	if (radioTextAlignStr == "left") {
		return RADIO_TEXT_ALIGN_LEFT;
	}
	else if (radioTextAlignStr == "right") {
		return RADIO_TEXT_ALIGN_RIGHT;
	}
	else {
		return RADIO_TEXT_ALIGN_INVALID;
	}
}