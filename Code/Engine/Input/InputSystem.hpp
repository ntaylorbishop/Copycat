#pragma once

#ifdef WIN32
#define PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Console/Console.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTS, DEFINES, AND TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int NUM_KEYS = 256;

#define NUM_KEY_0 0x60
#define NUM_KEY_1 0x61
#define NUM_KEY_2 0x62
#define NUM_KEY_3 0x63
#define NUM_KEY_4 0x64
#define NUM_KEY_5 0x65
#define NUM_KEY_6 0x66
#define NUM_KEY_7 0x67
#define NUM_KEY_8 0x68
#define NUM_KEY_9 0x69

//#define CREATE_SINGLETON(singleton, sType)

typedef std::pair<uint64_t, bool> FrameKeyPair;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum eMouseBtn {
	MOUSE_LEFT = 0,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INPUTSYSTEM CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InputSystem {
public:
	static void Initialize();
	static void Shutdown();

	static void Update(float deltaSeconds);


	static bool HasQuit();
	static void Quitting(bool isNowQuitting);
	 
	static bool GetKey(KEYCODE asKey);
	static bool GetKeyDown(KEYCODE asKey);
	static void SetKeyDownStatus(KEYCODE asKey, bool isNowDown);
	 
	static bool GetMouseBtn(eMouseBtn btn);
	static bool GetMouseBtnDown(eMouseBtn btn);
	static void SetMouseBtnDown(eMouseBtn btn, bool isDown);
	static bool GetMouseBtnUp(eMouseBtn btn);
	static void SetMouseBtnUp(eMouseBtn btn, bool isUp);
	 
	static void SetMouseWheelDelta(short wheelDelta);
	static bool GetMouseWheelUp();
	static bool GetMouseWheelDown();
	static IntVector2 GetRelativeCursorPosition();
	static bool IsMouseWithinQuad(const AABB2& quad);
	 
	static void SetCursorPosition(ScreenCoords pos);
	static ScreenCoords GetCursorPosition();
	static void HideMouseCursor();
	static void ShowMouseCursor();
	 
	//ENGINE-ONLY
	static void DisableInputToGame();
	static void EnableInputToGame();

private:
	//STRUCTORS
	InputSystem();

	//CONSOLE INPUT
	static bool GetKeyDownForConsole(KEYCODE asKey);
	static bool GetMouseWheelUpForConsole();
	static bool GetMouseWheelDownForConsole();

	FrameKeyPair m_currKeysDown[NUM_KEYS];

	bool m_quit						= false;
	bool m_rightMouseDown			= false;
	bool m_leftMouseDown			= false;
	bool m_middleMouseDown			= false;

	bool m_rightMouseUp				= false;
	bool m_leftMouseUp				= false;
	bool m_middleMouseUp			= false;

	int m_mouseWheelUpDelta			= 0;
	int m_mouseWheelDownDelta		= 0;

	bool m_isInputEnabledForGame	= true;

	uint64_t m_frameNumber			= 0;

	static InputSystem* s_theInputSystem;

	friend class Console;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WINDOWS MESSAGE HANDLING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam);