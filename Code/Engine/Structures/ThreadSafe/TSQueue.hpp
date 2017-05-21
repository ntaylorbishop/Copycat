#pragma once

#include <queue>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Structures/ThreadSafe/MutexLock.hpp"

template<typename T>
class TSQueue : public std::queue<T> {
public:
	TSQueue<T>() : std::queue<T>() { }

	template<typename T>
	void Enqueue(const T& value) {

		mutex.Lock();
		push(value);
		mutex.Unlock();
	}

	template<typename T>
	bool Dequeue(T& out) {

		bool result = false;
		mutex.Lock();
		if (!empty()) {
			out = front();
			pop();
			result = true;
		}

		mutex.Unlock();
		return result;
	}
private:
	MutexLock mutex;
};
