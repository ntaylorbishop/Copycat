#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Profiler/Profiler.hpp"
#include "Engine/EventSystem/Event.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Tools/FBX.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/General/Time/Clock.hpp"
#include "Engine/General/JobSystem/JobSystem.hpp"
#include "Engine/EventSystem/GlobalEventSystem.hpp"
#include "Engine/UI/UISystem.hpp"

BeirusEngine* BeirusEngine::s_theBeirusEngine = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INIT SHUTDOWN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void BeirusEngine::Initialize() {

	ASSERT_OR_DIE(s_theBeirusEngine == nullptr, "ERROR: BeirusEngine already initialized.");
	s_theBeirusEngine = new BeirusEngine();

	BeirusRenderer::InitDefaultProjectionMats();
	InputSystem::Initialize();
	BeirusRenderer::Initialize(RGBA(0.1f, 0.1f, 0.1f, 1.f));
	Console::Initialize(CONSOLE_KEYBIND);
	EventSystem::Initialize();
	UISystem::Initialize();

	#if MEMORY_TRACKING == 1
	RegisterCommandToConsole("memory_debug", "Toggles the memory debugger window", ShowMemoryDebugWindow);
	#elif MEMORY_TRACKING == 2
	RegisterCommandToConsole("memory_debug", "Toggles the memory debugger window", ShowMemoryDebugWindow);
	RegisterCommandToConsole("memory_flush", "Flushes callstacks of all active allocations to the output window", MemFlushCommand);
	#endif
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusEngine::Shutdown() {

	ASSERT_OR_DIE(s_theBeirusEngine != nullptr, "ERROR: BeirusEngine already shutdown.");

	for (size_t sdIdx = 0; sdIdx < s_theBeirusEngine->m_shutdownCallbacks.size(); sdIdx++) {
		s_theBeirusEngine->m_shutdownCallbacks[sdIdx]();
	}

	Console::Shutdown();
	InputSystem::Shutdown();
	BeirusRenderer::Shutdown();
	EventSystem::Shutdown();
	UISystem::Shutdown();

	delete s_theBeirusEngine;
	s_theBeirusEngine = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusEngine::Update(float deltaSeconds) {

	Profiler::PushFrame("BeirusEngineUpdate");

	s_theBeirusEngine->UpdateFrameNumber();

	s_theBeirusEngine->m_engineClock->Update(deltaSeconds);
	Console::Update(deltaSeconds);
	UISystem::Update(deltaSeconds);

	#if MEMORY_TRACKING != 0
	g_theMemoryManager->Update(deltaSeconds);
	#endif

	for (uint i = 0; i < s_theBeirusEngine->m_updateFuncsToCall.size(); i++) {
		s_theBeirusEngine->m_updateFuncsToCall[i](deltaSeconds);
	}

	Profiler::PopFrame();
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusEngine::Render() {

	#if MEMORY_TRACKING != 0
	g_theMemoryManager->Render();
	#endif

	Console::Render();
	UISystem::Render();
	//BeirusRenderer::Render();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE REGISTRATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusEngine::RegisterUpdateCallback(UpdateDelegate updateFunc) {
	s_theBeirusEngine->m_updateFuncsToCall.push_back(updateFunc);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusEngine::UnregisterUpdateCallback(UpdateDelegate updateFunc) {

	uint idx = 0;
	for (idx = 0; idx < s_theBeirusEngine->m_updateFuncsToCall.size(); idx++) {
		if (s_theBeirusEngine->m_updateFuncsToCall[idx] == updateFunc) {
			break;
		}
	}
	s_theBeirusEngine->m_updateFuncsToCall.erase(s_theBeirusEngine->m_updateFuncsToCall.begin() + idx);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE STRUCTORS AND INITIALIZATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
BeirusEngine::BeirusEngine()
	: m_frameNumber(INVALID_FRAME_NUMBER)
{

	//BeirusRenderer::InitializeOpenGLDefinitions();
	m_engineClock = new Clock();
}


//---------------------------------------------------------------------------------------------------------------------------
BeirusEngine::~BeirusEngine() {
	Font::DeinitFonts();
	BitmapFont::DeinitBitmapFonts();
	SpriteSheet::DeinitSpriteSheets();
	Texture::DeinitTextures();
	delete m_engineClock;
}

//---------------------------------------------------------------------------------------------------------------------------
void BeirusEngine::UpdateFrameNumber() {

	m_frameNumber++;
	if (m_frameNumber == INVALID_FRAME_NUMBER) {
		m_frameNumber = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//COMMANDS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ShowMemoryDebugWindow(Command& args) {

	UNREFERENCED_PARAMETER(args);

	#if MEMORY_TRACKING != 0
	g_theMemoryManager->Toggle();
	bool isEnabled = g_theMemoryManager->GetToggledStatus();

	if (isEnabled) {
		BeirusEngine::PrintOutputToConsole("Memory Debug Window is enabled.", CONSOLE_VERIFY);
	}
	else {
		BeirusEngine::PrintOutputToConsole("Memory Debug Window is disabled.", CONSOLE_VERIFY);
	}
	#endif
}

#if MEMORY_TRACKING == 2
void MemFlushCommand(Command& args) {
	UNREFERENCED_PARAMETER(args);
	MemFlush();
	BeirusEngine::PrintOutputToConsole("Memory flushed to output window.", CONSOLE_VERIFY);
}
#endif