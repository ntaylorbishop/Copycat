#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class LinearBuffer {
public:
	LinearBuffer(size_t bufferSize);
	~LinearBuffer();

	template <typename T>
	T* Allocate();


private:
	size_t m_bufferSize;
	void* data;

};