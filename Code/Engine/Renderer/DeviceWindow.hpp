#pragma once

#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TARGET_MAJOR 4
#define TARGET_MINOR 3

typedef HGLRC GLRenderContextHandle;
typedef HDC DeviceContextHandle;
typedef HWND ApplicationWindowHandle;

#if IS_WINDOWED
const int OFFSET_FROM_WINDOWS_DESKTOP	= 10;
const int WINDOW_PHYSICAL_WIDTH			= 1600;
const int WINDOW_PHYSICAL_HEIGHT		= 900;
const double VIEW_RIGHT					= 1600.0;
#else
const int OFFSET_FROM_WINDOWS_DESKTOP = 0;
const int WINDOW_PHYSICAL_WIDTH = 3840;
const int WINDOW_PHYSICAL_HEIGHT = 2160;
const double VIEW_RIGHT = 3840.f;
#endif

const double VIEW_LEFT = 0.0;
const double VIEW_BOTTOM = 0.0;
const double VIEW_TOP = VIEW_RIGHT * static_cast<double>(WINDOW_PHYSICAL_HEIGHT) / static_cast<double>(WINDOW_PHYSICAL_WIDTH);


extern HWND		g_hWnd;
extern HDC		g_displayDeviceContext;
extern HGLRC	g_openGLRenderingContext;

extern const char* APP_NAME;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEVICE WINDOW
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DeviceWindow {
public:
	//INIT
	static void CreateDeviceWindow(HINSTANCE applicationInstanceHandle);
	static void Shutdown();

	//GETTERS SETTERS
	static bool HasFocus();
	static void OnGainedFocus();
	static void OnLostFocus();

	static IntVector2 GetScreenDimensions() { return s_theDeviceWindow->m_screenDimensions; }
	static IntVector2 GetCursorDelta();

private:
	//STRUCTORS
	DeviceWindow(HINSTANCE applicationInstanceHandle);
	~DeviceWindow();

	//RENDERING CONTEXT CREATION
	DeviceContextHandle CreateApplicationWindow(HINSTANCE applicationInstanceHandle);
	GLRenderContextHandle CreateTemporaryContext(DeviceContextHandle context);
	bool CanCreateRealContext(int32_t majorVersionTarget, int32_t minorVersionTarget, DeviceContextHandle deviceContext);
	GLRenderContextHandle CreateRealContext(DeviceContextHandle hdc, GLRenderContextHandle oldContext);
	void DestroyRenderingContext(GLRenderContextHandle contextHandle);


	IntVector2 m_middleOfScreen;
	IntVector2 m_cursorDelta;
	IntVector2 m_screenDimensions;
	bool m_windowInFocus;

	static DeviceWindow* s_theDeviceWindow;
};