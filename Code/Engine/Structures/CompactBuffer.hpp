#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class CompactBuffer {
public:
	CompactBuffer(void* data, size_t size);

	template<typename T>
	T* Allocate() {

	}

	void* Front();
};

CBuffer buf;
buf.init(buf_data, size);

callstack_t *cs = buf.alloc<callstack_t>();
cs->frames = (void**)buf.get_front();