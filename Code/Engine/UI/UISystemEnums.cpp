#include "Engine/UI/Utils/UISystemEnums.hpp"

eWidgetState GetWidgetStateFromString(const String& widgetStateStr) {

	if (widgetStateStr == "Enabled") {
		return WIDGET_STATE_ENABLED;
	}
	else if (widgetStateStr == "Disabled") {
		return WIDGET_STATE_DISABLED;
	}
	else if (widgetStateStr == "Pressed") {
		return WIDGET_STATE_PRESSED;
	}
	else if (widgetStateStr == "Hover") {
		return WIDGET_STATE_HOVER;
	}
	else if (widgetStateStr == "Enabled-Selected") {
		return WIDGET_STATE_ENABLED_SELECTED;
	}
	else if (widgetStateStr == "Disabled-Selected") {
		return WIDGET_STATE_DISABLED_SELECTED;
	}
	else if (widgetStateStr == "Hover-Selected") {
		return WIDGET_STATE_HOVER_SELECTED;
	}
	else if (widgetStateStr == "Pressed-Selected") {
		return WIDGET_STATE_PRESSED_SELECTED;
	}
	else if (widgetStateStr == "Default") {
		return WIDGET_STATE_DEFAULT;
	}
	else {
		return WIDGET_STATE_NUM_WIDGET_STATES;
	}
}