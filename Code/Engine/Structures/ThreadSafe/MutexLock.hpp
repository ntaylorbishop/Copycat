#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class MutexLock {
public:
	//STRUCTORS
	MutexLock();
	~MutexLock();

	//LOCK UNLOCKING
	void Lock();
	void Unlock();

private:
	CRITICAL_SECTION cs;
};