/*
 * Based on code by Christopher Forseth
 */

#include "Engine/Renderer/DeviceWindow.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "ENgine/General/Core/WindowHandler.hpp"

const char* APP_NAME = "Sponza";

HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;

STATIC DeviceWindow* DeviceWindow::s_theDeviceWindow			= nullptr;

PFNWGLCREATECONTEXTATTRIBSARBPROC	wglCreateContextAttribsARB	= nullptr;
PFNWGLGETEXTENSIONSSTRINGARBPROC	wglGetExtensionsStringARB	= nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB		= nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT SHUTDOWN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void DeviceWindow::CreateDeviceWindow(HINSTANCE applicationInstanceHandle) {

	SetProcessDPIAware();
	s_theDeviceWindow = new DeviceWindow(applicationInstanceHandle);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void DeviceWindow::Shutdown() {

	delete s_theDeviceWindow;
	s_theDeviceWindow = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC bool DeviceWindow::HasFocus() {
	if (s_theDeviceWindow) {
		return s_theDeviceWindow->m_windowInFocus;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void DeviceWindow::OnGainedFocus() {
	if (s_theDeviceWindow) {
		s_theDeviceWindow->m_windowInFocus = true;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void DeviceWindow::OnLostFocus() {
	if (s_theDeviceWindow) {
		s_theDeviceWindow->m_windowInFocus = false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC IntVector2 DeviceWindow::GetCursorDelta() {

	ScreenCoords cursorPos = InputSystem::GetCursorPosition();
	InputSystem::SetCursorPosition(s_theDeviceWindow->m_middleOfScreen);
	s_theDeviceWindow->m_cursorDelta = cursorPos - s_theDeviceWindow->m_middleOfScreen;
	return s_theDeviceWindow->m_cursorDelta;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
DeviceWindow::DeviceWindow(HINSTANCE applicationInstanceHandle)
	: m_screenDimensions(IntVector2(WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT))
{

	g_theWindowHandler = new WindowHandler();
	m_middleOfScreen = (m_screenDimensions / 2);

	DeviceContextHandle deviceContext = CreateApplicationWindow(applicationInstanceHandle);
	//ASSERT_OR_DIE(deviceContext != NULL, "ERROR: Could not get device context.");

	GLRenderContextHandle tempContext = CreateTemporaryContext(deviceContext);
	//ASSERT_OR_DIE(tempContext != NULL, "ERROR: Could not create temporary device context.");

	if (!CanCreateRealContext(TARGET_MAJOR, TARGET_MINOR, deviceContext)) {
		DestroyRenderingContext(tempContext);
		//ERROR_AND_DIE("Cannot create OpenGL window with most recent versions.");
	}

	CreateRealContext(deviceContext, tempContext);
	DestroyRenderingContext(tempContext);
	tempContext = NULL;

	#if IS_WINDOWED
	glViewport(0, 0, 1600, 900);
	#else
	glViewport(0, 0, 3840, 2160);
	#endif

}


//---------------------------------------------------------------------------------------------------------------------------
DeviceWindow::~DeviceWindow() {

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RENDERING CONTEXT CREATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
DeviceContextHandle DeviceWindow::CreateApplicationWindow(HINSTANCE applicationInstanceHandle) {

	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	#if IS_WINDOWED
	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;
	#else
	const DWORD windowStyleFlags = WS_POPUP;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;
	#endif

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);

	RECT windowRect = { OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_WIDTH, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_HEIGHT };
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL
		);

	g_theWindowHandler->RegisterMainWindow(g_hWnd);


	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	g_displayDeviceContext = GetDC(g_hWnd);

	return g_displayDeviceContext;
}


//---------------------------------------------------------------------------------------------------------------------------
GLRenderContextHandle DeviceWindow::CreateTemporaryContext(DeviceContextHandle context) {

	PIXELFORMATDESCRIPTOR pixelFormatDesc;
	pixelFormatDesc.nSize = sizeof(pixelFormatDesc);
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.cColorBits = 32;
	pixelFormatDesc.cRedBits = 0;
	pixelFormatDesc.cRedShift = 0;
	pixelFormatDesc.cGreenBits = 0;
	pixelFormatDesc.cGreenShift = 0;
	pixelFormatDesc.cBlueBits = 0;
	pixelFormatDesc.cBlueShift = 0;
	pixelFormatDesc.cAlphaBits = 0;
	pixelFormatDesc.cAlphaShift = 0;
	pixelFormatDesc.cAccumBits = 0;
	pixelFormatDesc.cAccumRedBits = 0;
	pixelFormatDesc.cAccumGreenBits = 0;
	pixelFormatDesc.cAccumBlueBits = 0;
	pixelFormatDesc.cAccumAlphaBits = 0;
	pixelFormatDesc.cDepthBits = 0; // 24; Depth/Stencil handled by FBO
	pixelFormatDesc.cStencilBits = 0; // 8; DepthStencil handled by FBO
	pixelFormatDesc.cAuxBuffers = 0;
	pixelFormatDesc.iLayerType = PFD_MAIN_PLANE; // ignored now according to MSDN
	pixelFormatDesc.bReserved = 0;
	pixelFormatDesc.dwLayerMask = 0;
	pixelFormatDesc.dwVisibleMask = 0;
	pixelFormatDesc.dwDamageMask = 0;

	int32_t pixel_format = ChoosePixelFormat(context, &pixelFormatDesc);
	ASSERT_OR_DIE(pixel_format != NULL, "ERROR: Temporary window creation failed on getting pixel format.");
	ASSERT_OR_DIE(SetPixelFormat(context, pixel_format, &pixelFormatDesc), NULL);

	// Create a temp context so we can get the functions we need to make a proper one.
	GLRenderContextHandle tempContext = wglCreateContext(context);
	ASSERT_OR_DIE(tempContext != NULL, "ERROR: Temporary window creation failed on temp context creation.");

	wglMakeCurrent(context, tempContext);
	return tempContext;
}


//---------------------------------------------------------------------------------------------------------------------------
/* https://www.opengl.org/archives/resources/features/OGLextensions/ */
static int IsOGLExtensionSupported(const char* extension, DeviceContextHandle deviceContext) {

	const char* extensions = NULL;
	const char* start;
	const char* where;
	const char* terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return FALSE;
	extensions = wglGetExtensionsStringARB(deviceContext);
	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */
	start = extensions;
	for (;;) {
		where = strstr((const char *)start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return TRUE;
		start = terminator;
	}
	return FALSE;
}


//---------------------------------------------------------------------------------------------------------------------------
bool DeviceWindow::CanCreateRealContext(int32_t majorVersionTarget, int32_t minorVersionTarget, DeviceContextHandle deviceContext) {

	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

	// This now works!
	int32_t majorVersion, minorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	DebuggerPrintf("OpenGL Major Version: %i\n", majorVersion);
	DebuggerPrintf("OpenGL Minor Version: %i\n", minorVersion);

	if ((majorVersion > majorVersionTarget) || ((majorVersion >= majorVersionTarget) && (minorVersion >= minorVersionTarget))) {

		

		ASSERT_OR_DIE(wglGetExtensionsStringARB != nullptr, "ERROR: Cannot query for WGL extensions.");

		//Now check to see if those wgl extensions are available.
		ASSERT_OR_DIE(IsOGLExtensionSupported("WGL_ARB_create_context", deviceContext), "ERROR: Supported OpenGL version does not support WGL_ARB commands provided.");
		ASSERT_OR_DIE(IsOGLExtensionSupported("WGL_ARB_pixel_format", deviceContext), "ERROR: Supported OpenGL version does not support WGL_ARB commands provided.");

		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		return true;
	}

	ERROR_AND_DIE("Unsupported OpenGL Version or extensions.");
}


//---------------------------------------------------------------------------------------------------------------------------
GLRenderContextHandle DeviceWindow::CreateRealContext(DeviceContextHandle hdc, GLRenderContextHandle oldContext) {

	UNREFERENCED_PARAMETER(oldContext);
	//First, hook the ptr to create modern context

	// Now let's see
	int32_t const format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,    // FBO now handles the depth/stencil channel
		WGL_STENCIL_BITS_ARB, 8,   // FBO now handles the depth/stencil channel
		NULL, NULL,
	};

	// int32_t pixel_format;
	const size_t cTotalFormats = 128;
	int32_t formats[cTotalFormats];
	int32_t pixel_format = 0;
	uint32_t format_count = 0;

	BOOL succeeded = wglChoosePixelFormatARB(hdc, format_attribs, nullptr, cTotalFormats, formats, (UINT*)&format_count);
	ASSERT_OR_DIE(succeeded == TRUE, "ERROR: Cannot grab pixel format on new rendering context.");

	// Loop through returned formats, till we find one that works
	for (uint32_t i = 0; i < format_count; ++i) {
		pixel_format = formats[i];
		SetPixelFormat(hdc, pixel_format, NULL);
	}
	//ASSERT_OR_DIE(boundAPixelFormat == TRUE, "Failed to bind pixel format.");

	int const attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		#if defined(_DEBUG)
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB || WGL_CONTEXT_DEBUG_BIT_ARB,
		#else
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		#endif
		0, 0
	};


	GLRenderContextHandle context = wglCreateContextAttribsARB(hdc, NULL, attribs);
	ASSERT_OR_DIE(context != NULL, "ERROR: Cannot create new rendring context.");

	wglMakeCurrent(NULL, NULL);
	wglMakeCurrent(hdc, context);

	String output = BeirusRenderer::GetCurrentVersionOfOpenGL();

	return context;
}


//---------------------------------------------------------------------------------------------------------------------------
void DeviceWindow::DestroyRenderingContext(GLRenderContextHandle contextHandle) {

	if (contextHandle != NULL) {	
		wglDeleteContext(contextHandle);
	}
}