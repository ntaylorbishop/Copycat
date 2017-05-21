#pragma once

template <typename T>
class SharedPointer {
public:
	SharedPointer<T>() : m_ptr(nullptr), m_refCount(0) { }
	SharedPointer<T>(T* ptr);
	~SharedPointer();

	SharedPointer(const SharedPointer<T>& other);
	SharedPointer<T> operator=(const SharedPointer<T>& other);
	//T* operator->();

	T* m_ptr;
private:
	size_t m_refCount;
};

//---------------------------------------------------------------------------------------------------------------------------
//TEMPLATE FUNCS
//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
SharedPointer<T>::SharedPointer(T* ptr)
	: m_ptr(ptr)
	, m_refCount(1)
{ }
template <typename T>
SharedPointer<T>::~SharedPointer() {
	if (--m_refCount == 0)
		delete m_ptr;
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& other) {
	const_cast<SharedPointer<T>&>(other).m_refCount++;
	m_refCount = other.m_refCount;
	m_ptr = other.m_ptr;
}

template <typename T>
inline SharedPointer<T> SharedPointer<T>::operator=(const SharedPointer<T>& other) {
	const_cast<SharedPointer<T>&>(other).m_refCount++;
	m_refCount--;
	m_ptr = other.m_ptr;
}

/*
template <typename T>
inline T* SharedPointer<T>::operator->() {

	ASSERT_OR_DIE(m_ptr, "ERROR: You tried to dereference a nullptr ya goof");
	return m_ptr;
}*/

/*
SharedPointer<Entity*> sPtr1;
SharedPointer<Entity*> sPtr2;
sPtr1 = sPtr2;

Foo(sPtr1);

void Foo(const SharedPointer<Entity*>& ptr);*/