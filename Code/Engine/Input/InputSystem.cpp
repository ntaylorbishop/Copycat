#include "Engine/Input/InputSystem.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Core/WindowHandler.hpp"
#include "Engine/Renderer/DeviceWindow.hpp"

STATIC InputSystem* InputSystem::s_theInputSystem = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::Initialize() {

	ASSERT_OR_DIE(s_theInputSystem == nullptr, "ERROR: InputSystem already initialized");
	s_theInputSystem = new InputSystem();
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::Shutdown() {

	ASSERT_OR_DIE(s_theInputSystem == nullptr, "ERROR: InputSystem already initialized");
	delete s_theInputSystem;
	s_theInputSystem = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::Update(float deltaSeconds) {

	UNUSED(deltaSeconds);

	s_theInputSystem->m_leftMouseUp = false;
	s_theInputSystem->m_middleMouseUp = false;
	s_theInputSystem->m_rightMouseUp = false;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC PUBLIC INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::HasQuit() {
	return s_theInputSystem->m_quit;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetKey(KEYCODE asKey) {
	if (s_theInputSystem->m_isInputEnabledForGame) {
		return s_theInputSystem->m_currKeysDown[asKey].second;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetKeyDown(KEYCODE asKey) {

	if (s_theInputSystem->m_isInputEnabledForGame) {
		if ((BeirusEngine::GetFrameNumber() - s_theInputSystem->m_currKeysDown[asKey].first) <= 1 && s_theInputSystem->m_currKeysDown[asKey].second == true) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::SetKeyDownStatus(KEYCODE asKey, bool isNowDown) {

	if (isNowDown) {
		s_theInputSystem->m_currKeysDown[asKey].first = BeirusEngine::GetFrameNumber();
	}

	s_theInputSystem->m_currKeysDown[asKey].second = isNowDown;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetMouseBtn(eMouseBtn btn) {
	if (btn == MOUSE_LEFT)
		return s_theInputSystem->m_leftMouseDown;
	else if (btn == MOUSE_MIDDLE)
		return s_theInputSystem->m_middleMouseDown;
	else if (btn == MOUSE_RIGHT)
		return s_theInputSystem->m_rightMouseDown;
	else {
		SystemDialogue_Okay("Tried to get an invalid mouse key", "A function tried to see if an invalid mouse button was down.", SEVERITY_WARNING);
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetMouseBtnDown(eMouseBtn btn) {
	if (btn == MOUSE_LEFT) {
		bool b = s_theInputSystem->m_leftMouseDown;
		s_theInputSystem->m_leftMouseDown = false;
		return b;
	}
	else if (btn == MOUSE_MIDDLE) {
		bool b = s_theInputSystem->m_middleMouseDown;
		s_theInputSystem->m_middleMouseDown = false;
		return b;
	}
	else if (btn == MOUSE_RIGHT) {
		bool b = s_theInputSystem->m_rightMouseDown;
		s_theInputSystem->m_rightMouseDown = false;
		return b;
	}
	else {
		SystemDialogue_Okay("Tried to get an invalid mouse key", "A function tried to see if an invalid mouse button was down.", SEVERITY_WARNING);
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC IntVector2 InputSystem::GetRelativeCursorPosition() {
	return g_theWindowHandler->GetMainWindow().GetRelativeMousePosition();
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::IsMouseWithinQuad(const AABB2& quad) {
	IntVector2 relMousePos = GetRelativeCursorPosition();

	return (relMousePos.x >= quad.mins.x
		&& relMousePos.x <= quad.maxs.x
		&& relMousePos.y >= quad.mins.y
		&& relMousePos.y <= quad.maxs.y);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::SetMouseBtnDown(eMouseBtn btn, bool isDown) {
	if (btn == MOUSE_LEFT)
		s_theInputSystem->m_leftMouseDown = isDown;
	else if (btn == MOUSE_MIDDLE)
		s_theInputSystem->m_middleMouseDown = isDown;
	else if (btn == MOUSE_RIGHT)
		s_theInputSystem->m_rightMouseDown = isDown;
	else {
		SystemDialogue_Okay("Tried to get an invalid mouse key", "A function tried to see if an invalid mouse button was down.", SEVERITY_WARNING);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetMouseBtnUp(eMouseBtn btn) {

	switch (btn) {
	case MOUSE_LEFT:
		return s_theInputSystem->m_leftMouseUp;
	case MOUSE_MIDDLE:
		return s_theInputSystem->m_middleMouseUp;
	case MOUSE_RIGHT:
		return s_theInputSystem->m_rightMouseUp;
	default:
		ERROR_AND_DIE("Something went horribly wrong with the input system trying to GetMouseBtnUp");
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::SetMouseBtnUp(eMouseBtn btn, bool isUp) {
	switch (btn) {
	case MOUSE_LEFT:
		s_theInputSystem->m_leftMouseUp = isUp;
		return;
	case MOUSE_MIDDLE:
		s_theInputSystem->m_middleMouseUp = isUp;
		return;
	case MOUSE_RIGHT:
		s_theInputSystem->m_rightMouseUp = isUp;
		return;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::SetMouseWheelDelta(short wheelDelta) {
	s_theInputSystem->m_mouseWheelUpDelta = (int)wheelDelta;
	s_theInputSystem->m_mouseWheelDownDelta = (int)wheelDelta;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetMouseWheelUp() {
	int wheel = s_theInputSystem->m_mouseWheelUpDelta;
	s_theInputSystem->m_mouseWheelUpDelta = 0;
	return wheel > 0;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetMouseWheelDown() {
	int wheel = s_theInputSystem->m_mouseWheelDownDelta;
	s_theInputSystem->m_mouseWheelDownDelta = 0;
	return wheel < 0;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::Quitting(bool isNowQuitting) {
	s_theInputSystem->m_quit = isNowQuitting;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::SetCursorPosition(ScreenCoords pos) {
	SetCursorPos(pos.x, pos.y);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC ScreenCoords InputSystem::GetCursorPosition() {
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	return ScreenCoords((int)cursorPos.x, (int)cursorPos.y);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::HideMouseCursor() {
	ShowCursor(false);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::ShowMouseCursor() {
	ShowCursor(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC ENGINE-ONLY DISABLE INPUT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::DisableInputToGame() {
	s_theInputSystem->m_isInputEnabledForGame = false;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void InputSystem::EnableInputToGame() {
	s_theInputSystem->m_isInputEnabledForGame = true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE MEMBER FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
InputSystem::InputSystem()
	: m_isInputEnabledForGame(true)
	, m_leftMouseDown(false)
	, m_middleMouseDown(false)
	, m_rightMouseDown(false)
	, m_rightMouseUp(false)
	, m_leftMouseUp(false)
	, m_middleMouseUp(false)
	, m_quit(false)
{

	for (int i = 0; i < 255; i++) {
		m_currKeysDown[i].first = INVALID_FRAME_NUMBER;
		m_currKeysDown[i].second = false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetKeyDownForConsole(KEYCODE asKey) {

	if ((BeirusEngine::GetFrameNumber() - s_theInputSystem->m_currKeysDown[asKey].first) <= 1 && s_theInputSystem->m_currKeysDown[asKey].second == true) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetMouseWheelUpForConsole() {

	int wheel = s_theInputSystem->m_mouseWheelUpDelta;
	s_theInputSystem->m_mouseWheelUpDelta = 0;
	return wheel > 0;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool InputSystem::GetMouseWheelDownForConsole() {

	int wheel = s_theInputSystem->m_mouseWheelDownDelta;
	s_theInputSystem->m_mouseWheelDownDelta = 0;
	return wheel < 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WINDOWS MESSAGE HANDLING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam) {
	unsigned char asKey = (unsigned char)wParam;
	switch (wmMessageCode) {
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		InputSystem::Quitting(true);
		return 0;
	case WM_KEYDOWN:
		InputSystem::SetKeyDownStatus(asKey, true);

		if (asKey == VK_ESCAPE) {
			if (!Console::IsConsoleOpen()) {
				InputSystem::Quitting(true);
				return 0;
			}
		}
		break;

	case WM_CHAR:
		Console::ConsumeCharacterInput(asKey);
		break;

	case WM_KEYUP:
		InputSystem::SetKeyDownStatus(asKey, false);
		break;

	case WM_SETFOCUS:
		DeviceWindow::OnGainedFocus();
		break;

	case WM_KILLFOCUS:
		DeviceWindow::OnLostFocus();
		break;

	case WM_LBUTTONDOWN:
		InputSystem::SetMouseBtnDown(MOUSE_LEFT, true);
		break;

	case WM_LBUTTONUP:
		InputSystem::SetMouseBtnDown(MOUSE_LEFT, false);
		InputSystem::SetMouseBtnUp(MOUSE_LEFT, true);
		break;

	case WM_MBUTTONDOWN:
		InputSystem::SetMouseBtnDown(MOUSE_MIDDLE, true);
		break;

	case WM_MBUTTONUP:
		InputSystem::SetMouseBtnDown(MOUSE_MIDDLE, false);
		break;

	case WM_RBUTTONDOWN:
		InputSystem::SetMouseBtnDown(MOUSE_RIGHT, true);
		break;

	case WM_RBUTTONUP:
		InputSystem::SetMouseBtnDown(MOUSE_RIGHT, false);
		break;

	case WM_MOUSEWHEEL:
		InputSystem::SetMouseWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	}

	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}