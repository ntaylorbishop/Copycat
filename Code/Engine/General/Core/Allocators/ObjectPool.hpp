#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCT
//---------------------------------------------------------------------------------------------------------------------------
struct Page {
	Page* m_next;
};

//---------------------------------------------------------------------------------------------------------------------------
//OBJECT POOL
//---------------------------------------------------------------------------------------------------------------------------
template<typename T>
class ObjectPool {
public:
	ObjectPool() : m_freeList(nullptr), m_begOfBuffer(nullptr) { }
	ObjectPool(size_t initialCount) 
		: m_freeList(nullptr)
		, m_begOfBuffer(nullptr)
		, m_numAllocs(0)
	{
		size_t totalSize = sizeof(T) * initialCount;

		byte* buffer = (byte*)malloc(totalSize);
		m_begOfBuffer = (Page*)buffer;
		m_begOfBuffer->m_next = nullptr;
		m_freeList = m_begOfBuffer;

		for (size_t i = 0; i < initialCount; i++) {
			Page* nPage = (Page*)buffer;
			nPage->m_next = m_freeList;
			m_freeList = nPage;

			buffer += sizeof(T);
		}
	}
	~ObjectPool() { }

	void Deconstruct() { free(m_begOfBuffer); }

	void Free(T* obj) {

		obj->~T();

		Page* temp = (Page*)obj;
		temp->m_next = m_freeList;
		m_freeList = temp;
	}

	T* Create() {
		//LOCK
		T* nObject = (T*)m_freeList;
		m_freeList = m_freeList->m_next;
		//UNLOCK
		new(nObject) T();
		//LOCK
		m_numAllocs++; //ATOMIC INCREMENT DECREMENT
		//UNLOCK
		return nObject;
	}
	

private:
	void* Allocate(size_t size) {
		return nPage;
	}

	Page* m_freeList;
	Page* m_begOfBuffer;

	uint m_numAllocs;
};