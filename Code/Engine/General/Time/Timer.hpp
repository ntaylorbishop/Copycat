#pragma once
#include "Engine/General/Time/Clock.hpp"
#pragma warning(push, 1)
#include "ThirdParty/FastDelegate.h"
#pragma warning(pop)

using namespace fastdelegate;
typedef FastDelegate0<> CallDelegate;

class Timer {
public:
	static void CreateTimerByFunctionHandle(float expireTime, CallDelegate fptr);
	static void CreateTimerWithTick(float expireTime, CallDelegate fptr);



private:
	Timer(float expireTime, CallDelegate fptr, bool calledEveryFrame);

	Clock m_clock;
};