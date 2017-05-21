#include "XboxController.hpp"

XboxController::XboxController() {
	this->m_controllerNumber = 0;

	m_leftTrigger = 0;
	m_rightTrigger = 0;
	m_connected = false;

	m_lThumbPolar = PolarCoords(0.f, 0.f);
	m_rThumbPolar = PolarCoords(0.f, 0.f);

	m_frameNumber = 0;
	for (int i = 0; i < 14; i++)
		m_button_frameNumber[i] = -1;
}
XboxController::XboxController(int controllerNumber) {
	this->m_controllerNumber = controllerNumber;

	m_leftTrigger = 0;
	m_rightTrigger = 0;
	m_connected = false;

	m_lThumbPolar = PolarCoords(0.f, 0.f);
	m_rThumbPolar = PolarCoords(0.f, 0.f);

	m_frameNumber = 0;
	for (int i = 0; i < 14; i++)
		m_button_frameNumber[i] = -1;
}

void XboxController::Update() {
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState)); 
	DWORD errorStatus = XInputGetState(m_controllerNumber, &xboxControllerState);

	if (errorStatus == ERROR_SUCCESS) {
		if (m_frameNumber == 0) {
			m_prevButtons = xboxControllerState.Gamepad.wButtons;
			m_buttons = xboxControllerState.Gamepad.wButtons;
		}
		else {
			m_prevButtons = m_buttons;
			m_buttons = xboxControllerState.Gamepad.wButtons;
		}


		m_frameNumber++;

		//Check to see if a button went down this frame
		if (((m_buttons & 0x0001)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[0] = m_frameNumber;
		if (((m_buttons & 0x0002)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[1] = m_frameNumber;
		if (((m_buttons & 0x0004)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[2] = m_frameNumber;
		if (((m_buttons & 0x0008)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[3] = m_frameNumber;
		if (((m_buttons & 0x0010)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[4] = m_frameNumber;
		if (((m_buttons & 0x0020)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[5] = m_frameNumber;
		if (((m_buttons & 0x0040)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[6] = m_frameNumber;
		if (((m_buttons & 0x0080)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[7] = m_frameNumber;
		if (((m_buttons & 0x0100)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[8] = m_frameNumber;
		if (((m_buttons & 0x0200)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[9] = m_frameNumber;
		if (((m_buttons & 0x1000)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[10] = m_frameNumber;
		if (((m_buttons & 0x2000)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[11] = m_frameNumber;
		if (((m_buttons & 0x4000)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[12] = m_frameNumber;
		if (((m_buttons & 0x8000)) != 0 && m_prevButtons != m_buttons) m_button_frameNumber[13] = m_frameNumber;

		m_lThumb.x = (float)xboxControllerState.Gamepad.sThumbLX;
		m_lThumb.y = (float)xboxControllerState.Gamepad.sThumbLY;
		m_rThumb.x = xboxControllerState.Gamepad.sThumbRX;
		m_rThumb.y = xboxControllerState.Gamepad.sThumbRY;
		m_leftTrigger = (int)xboxControllerState.Gamepad.bLeftTrigger;
		m_rightTrigger = (int)xboxControllerState.Gamepad.bRightTrigger;

		UpdateLeftStick();
		UpdateRightStick();

		m_connected = true;
	}
	else if (errorStatus == ERROR_DEVICE_NOT_CONNECTED) {
		m_connected = false;
	}
}
void XboxController::UpdateLeftStick() {
	float xNorm = (float)m_lThumb.x / 32000.f;
	float yNorm = (float)m_lThumb.y / 32000.f;

	Vector2 leftThumbNorm = Vector2(xNorm, yNorm);
	float radius = leftThumbNorm.Length();
	float angle = atan2(yNorm, xNorm);

	float radiusMapped = RangeMap(radius, .3f, .976f, 0.f, 1.f);
	radiusMapped = ClampFloat(radiusMapped, 0.f, 1.f);

	m_lThumbPolar = PolarCoords(radiusMapped, angle);
}
void XboxController::UpdateRightStick() {
	float xNorm = (float)m_rThumb.x / 32000.f;
	float yNorm = (float)m_rThumb.y / 32000.f;

	Vector2 leftThumbNorm = Vector2(xNorm, yNorm);
	float radius = leftThumbNorm.Length();
	float angle = atan2(yNorm, xNorm);

	float radiusMapped = RangeMap(radius, .3f, .976f, 0.f, 1.f);
	radiusMapped = ClampFloat(radiusMapped, 0.f, 1.f);

	m_rThumbPolar = PolarCoords(radiusMapped, angle);
}

bool XboxController::GetButton(eXboxBtn btn) const {
	if      (btn == XB_DPAD_UP)     return ((m_buttons & 0x0001) != 0);
	else if (btn == XB_DPAD_DOWN)   return ((m_buttons & 0x0002) != 0);
	else if (btn == XB_DPAD_LEFT)   return ((m_buttons & 0x0004) != 0);
	else if (btn == XB_DPAD_RIGHT)  return ((m_buttons & 0x0008) != 0);
	else if (btn == XB_START)       return ((m_buttons & 0x0010) != 0);
	else if (btn == XB_BACK)        return ((m_buttons & 0x0020) != 0);
	else if (btn == XB_LEFT_THUMB)  return ((m_buttons & 0x0040) != 0);
	else if (btn == XB_RIGHT_THUMB) return ((m_buttons & 0x0080) != 0);
	else if (btn == XB_LB)          return ((m_buttons & 0x0100) != 0);
	else if (btn == XB_RB)          return ((m_buttons & 0x0200) != 0);
	else if (btn == XB_A)           return ((m_buttons & 0x1000) != 0);
	else if (btn == XB_B)           return ((m_buttons & 0x2000) != 0);
	else if (btn == XB_X)           return ((m_buttons & 0x4000) != 0);
	else if (btn == XB_Y)           return ((m_buttons & 0x8000) != 0);
	else
		return false;
}
bool XboxController::GetButtonDown(eXboxBtn btn) const {
	if      (btn == XB_DPAD_UP)     return (m_button_frameNumber[0] == m_frameNumber);
	else if (btn == XB_DPAD_DOWN)   return (m_button_frameNumber[1] == m_frameNumber);
	else if (btn == XB_DPAD_LEFT)   return (m_button_frameNumber[2] == m_frameNumber);
	else if (btn == XB_DPAD_RIGHT)  return (m_button_frameNumber[3] == m_frameNumber);
	else if (btn == XB_START)       return (m_button_frameNumber[4] == m_frameNumber);
	else if (btn == XB_BACK)        return (m_button_frameNumber[5] == m_frameNumber);
	else if (btn == XB_LEFT_THUMB)  return (m_button_frameNumber[6] == m_frameNumber);
	else if (btn == XB_RIGHT_THUMB) return (m_button_frameNumber[7] == m_frameNumber);
	else if (btn == XB_LB)          return (m_button_frameNumber[8] == m_frameNumber);
	else if (btn == XB_RB)          return (m_button_frameNumber[9] == m_frameNumber);
	else if (btn == XB_A)           return (m_button_frameNumber[10] == m_frameNumber);
	else if (btn == XB_B)           return (m_button_frameNumber[11] == m_frameNumber);
	else if (btn == XB_X)           return (m_button_frameNumber[12] == m_frameNumber);
	else if (btn == XB_Y)           return (m_button_frameNumber[13] == m_frameNumber);
	else
		return false;
}

PolarCoords XboxController::GetLeftStickPositionPolar() {
	return m_lThumbPolar;
}
PolarCoords XboxController::GetRightStickPositionPolar() {
	return m_rThumbPolar;
}
Vector2 XboxController::GetLeftStickPosition() {
	return Vector2(m_lThumbPolar.radius * cos(m_lThumbPolar.theta), m_lThumbPolar.radius * sin(m_rThumbPolar.theta));
}
Vector2 XboxController::GetRightStickPosition() {
	return Vector2(m_rThumbPolar.radius * cos(m_rThumbPolar.theta), m_rThumbPolar.radius * sin(m_rThumbPolar.theta));
}

float XboxController::GetLeftTriggerValue() const {
	float lTriggerCast = (float)m_leftTrigger;
	return RangeMap(lTriggerCast, 0.f, 255.f, 0.f, 1.f);
}
float XboxController::GetRightTriggerValue() const {
	float rTriggerCast = (float)m_rightTrigger;
	return RangeMap(rTriggerCast, 0.f, 255.f, 0.f, 1.f);
}

bool XboxController::IsConnected() {
	return m_connected;
}