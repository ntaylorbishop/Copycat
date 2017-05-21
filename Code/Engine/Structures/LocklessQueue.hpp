#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Core/Structures/SharedPointer.hpp"

template <typename T>
struct Node {
	T* m_data;
	Node* m_next;
};

template <typename T>
class LocklessQueue {
public:
	LocklessQueue<T>() { }

	bool IsEmpty() { return m_front->m_next != nullptr; }

	void Enqueue(const T& data);
	bool Dequeue(T* out);

private:
	SharedPointer<Node<T>> m_front;
	SharedPointer<Node<T>> m_back;
};

//---------------------------------------------------------------------------------------------------------------------------
//TEMPLATE FUNCS
//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void LocklessQueue<T>::Enqueue(const T& data) {

	UNREFERENCED_PARAMETER(data);

/*
	SharedPointer<Node<T>> nSPtr = SharedPointer<Node<T>>(new Node<T>());
	memcpy(nSPtr.m_ptr->m_data, data, sizeof(data));

	for(;;) {
		SharedPointer<Node<T>> oldBack = m_back;

		if (InterlockedCompareExchange((uint32_t*)m_back.m_ptr->m_next, (uint32_t)nSPtr.m_ptr, (uint32_t)oldBack.m_ptr->m_next) == 0) {
			InterlockedCompareExchange((uint32_t*)m_back.m_ptr, (uint32_t)oldBack.m_ptr->m_next, (uint32_t)oldBack.m_ptr);
			break;
		}
		else {
			InterlockedCompareExchange((uint32_t*)m_back.m_ptr, (uint32_t)oldBack.m_ptr->m_next, (uint32_t)oldBack.m_ptr);
		}
	}*/
}

template <typename T>
bool LocklessQueue<T>::Dequeue(T* out) {

	UNREFERENCED_PARAMETER(out);

/*
	SharedPointer<Node<T>> oldFront = m_front;
	//Could cause problems if someone else deletes when we try to do this
	//Use ref counting! Node *old_front = Acquire(front);
	SharedPointer<Node<T>> oldNext = oldFront.m_ptr->m_next;
	while (oldNext.m_ptr->m_data != nullptr) {
		if (InterlockedCompareExchange((uint32_t*)m_front.m_ptr, (uint32_t)oldNext.m_ptr, (uint32_t)oldFront.m_ptr) == (uint32_t)oldFront.m_ptr) {
			out = oldNext.m_ptr->m_data; //List is offset by 1
			delete oldFront.m_ptr;
			return true;
		}

		oldFront = m_front;
		//Could cause problems if someone else deletes when we try to do this
		oldNext = SharedPointer<Node<T>>(oldFront.m_ptr->m_next);
	}

	return false;*/
	return false;
}