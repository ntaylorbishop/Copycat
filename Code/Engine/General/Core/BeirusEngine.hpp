#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INCLUDES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
#include "Engine/General/Core/EngineCommon.hpp"
#include <vector>
#include <string>
#pragma warning(push, 1)
#include "ThirdParty/FastDelegate.h"
#pragma warning(pop)
using namespace fastdelegate;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Clock;
class Command;
#define INVALID_FRAME_NUMBER 0xFFFFFFFFFFFFFFFF

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS DEFINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void(*console_commmand_cb)(Command&);
typedef FastDelegate1<float> UpdateDelegate;
typedef void(ShutdownCallback)();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BEIRUSENGINE CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BeirusEngine {
public:
	//INIT SHUTDOWN
	static void Initialize();
	static void Shutdown();

	static bool HasBeenInitialized()	{ return s_theBeirusEngine != nullptr; }

	//UPDATE RENDER
	static void Update(float deltaSeconds);
	static void Render();
	
	//CALLBACK REGISTRATION
	static void RegisterUpdateCallback(UpdateDelegate updateFunc);
	static void UnregisterUpdateCallback(UpdateDelegate updateFunc);

	static void RegisterShutdownCallback(ShutdownCallback* sysShutdownCB);
	 
	//GETTERS SETTERS
	static Clock* GetEngineClock()		{ return s_theBeirusEngine->m_engineClock; }
	static uint64_t GetFrameNumber()	{ return s_theBeirusEngine->m_frameNumber; }

private:
	//STRUCTORS
	BeirusEngine();
	~BeirusEngine();

	void UpdateFrameNumber();


	std::vector<UpdateDelegate> m_updateFuncsToCall;
	std::vector<ShutdownCallback*> m_shutdownCallbacks;
	Clock* m_engineClock = nullptr;

	uint64_t m_frameNumber = 0;

	static BeirusEngine* s_theBeirusEngine;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC inline void BeirusEngine::RegisterShutdownCallback(ShutdownCallback* sysShutdownCB) {
	s_theBeirusEngine->m_shutdownCallbacks.push_back(sysShutdownCB);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OUTSIDE OF CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
#if MEMORY_TRACKING == 1
void ShowMemoryDebugWindow(Command& args);
#elif MEMORY_TRACKING == 2
void ShowMemoryDebugWindow(Command& args);
void MemFlushCommand(Command& args);
#endif