#include "Engine/General/Core/Window.hpp"
#include "Engine/Math/MathUtils.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Window::Window() 
	: m_window(nullptr)
	, m_name("NULL")
{ }
Window::Window(HWND window, String name) 
	: m_window(window)
	, m_name(name)
{ 
	m_windowSize = GetWindowSize();
}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS
//---------------------------------------------------------------------------------------------------------------------------
String Window::GetName() {
	return m_name;
}
IntVector2 Window::GetWindowOffset() {
	IntVector2 borderSize;
	IntVector2 windowSize;

	RECT clientRect;
	RECT windowRect;

	GetClientRect(m_window, &clientRect);
	GetWindowRect(m_window, &windowRect);

	return IntVector2();
}
IntVector2 Window::GetRelativeMousePosition() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_window, &p);

	IntVector2 relMousePos = IntVector2(p.x, m_windowSize.y - p.y);

	float x = RangeMap((float)relMousePos.x, 0.f, (float)m_windowSize.x, 0.f, 1920.f);
	float y = RangeMap((float)relMousePos.y, 0.f, (float)m_windowSize.y, 0.f, 1080.f);

	return IntVector2((int)x, (int)y);
}
bool Window::MouseIsInWindow() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_window, &p);
	IntVector2 relMousePos = IntVector2(p.x, p.y);

	if (relMousePos.x > 0 && relMousePos.y > 0 && relMousePos.x < m_windowSize.x && relMousePos.y < m_windowSize.y) {
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC IntVector2 Window::GetDesktopResolution() {
	RECT desktop;

	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	return IntVector2(desktop.right, desktop.bottom);
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
IntVector2 Window::GetWindowSize() {
	RECT clientRect;
	GetClientRect(m_window, &clientRect);

	Vector2 windowSize = Vector2((float)clientRect.right, (float)clientRect.bottom);

	return IntVector2(clientRect.right, clientRect.bottom);
}