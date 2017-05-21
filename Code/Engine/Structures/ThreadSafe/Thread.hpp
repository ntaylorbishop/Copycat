#pragma once

#include <thread>
#include <chrono>
#include "Engine/General/Core/EngineCommon.hpp"

typedef void(ThreadFP)();
typedef void(UserDataFP)(void* userData);

class Thread {
public:
	Thread() { }
	Thread(ThreadFP* fpStart) { m_thread = std::thread(fpStart); }
	Thread(UserDataFP* fpStart, void* userData) { m_thread = std::thread(fpStart, userData); }

	void Join() { m_thread.join(); }
	void Detach() { m_thread.detach(); }
	static void Sleep(uint milliseconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); }
	static void YieldForMilliseconds(uint milliseconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); }

private:
	std::thread m_thread;
};