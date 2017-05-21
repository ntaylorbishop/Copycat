#include "Engine/General/Core/WindowHandler.hpp"

WindowHandler* g_theWindowHandler = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
WindowHandler::WindowHandler() {

}

//---------------------------------------------------------------------------------------------------------------------------
//WINDOW REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
bool WindowHandler::RegisterMainWindow(HWND mainWindow) {
	m_mainWindow = Window(mainWindow, "Main");
	return true;
}
bool WindowHandler::RegisterWindow(HWND newWindow, String name) {
	m_registeredWindows.push_back(Window(newWindow, name));
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//GET WINDOWS
//---------------------------------------------------------------------------------------------------------------------------
Window WindowHandler::GetMainWindow() {
	return m_mainWindow;
}
Window WindowHandler::GetWindowByName(String name) {
	for (unsigned int i = 0; i < m_registeredWindows.size(); i++) {
		if (m_registeredWindows[i].GetName() == name) {
			return m_registeredWindows[i];
		}
	}

	return Window();
}