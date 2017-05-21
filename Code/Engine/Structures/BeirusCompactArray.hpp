#pragma once


/*
 *	Acts as a normal array, but moves the last element to the position of a delete element when calling Remove()
 *
 *  Rules:
 *		Cannot be copy constructed (Pass by const-reference)
 *		If using non-pointer element, 
 */

const size_t	BEIRUSCOMPACTARRAY_MAX_ELEMENTS = 1000000;
const uint8_t	AMORTIZED_CONST_GROWTH_FACTOR	= 3;

typedef bool(RemoveComparator)(const void*);


template <typename T>
class BeirusCompactArray {
public:
	//STRUCTORS
	BeirusCompactArray();
	BeirusCompactArray(size_t startingCapacity);
	BeirusCompactArray(const BeirusCompactArray& other) = delete;
	~BeirusCompactArray();
	void operator=(const BeirusCompactArray& other) = delete;

	//ADD REMOVE
	void Add(const T& newElem);
	void Remove(size_t idx);
	void RemoveIf(RemoveComparator* cbRemove);

	//GET
	size_t Size() const { return m_size; }
	T& operator[](int idx);
	T operator[](int idx) const;

private:
	//UTILS
	void Resize();

	size_t	m_capacity	= 1;
	size_t	m_size		= 0;
	T*		m_arr		= nullptr;
};

//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEMPLATE INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
BeirusCompactArray<T>::BeirusCompactArray() { 
	m_arr = (T*)malloc(m_capacity * sizeof(T));
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
BeirusCompactArray<T>::BeirusCompactArray(size_t startingCapacity) 
	: m_capacity(startingCapacity)
{ 
	m_arr = (T*)malloc(m_capacity * sizeof(T));
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
BeirusCompactArray<T>::~BeirusCompactArray() {

	for (size_t arrIdx = 0; arrIdx < m_size; arrIdx++) {
		m_arr[arrIdx].~T();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADD REMOVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void BeirusCompactArray<T>::Add(const T& newElem) {

	if (m_size == m_capacity) {
		Resize();
	}


	::new(m_arr + m_size) T();
	m_arr[m_size] = newElem;

	//todo;
	//Not copying over, so the copy is destructing this one's elements
	m_size++;
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void BeirusCompactArray<T>::Remove(size_t idx) {

	memcpy_s(m_arr + idx, sizeof(T), m_arr + m_size, sizeof(T));
	m_size--;
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void BeirusCompactArray<T>::RemoveIf(RemoveComparator* cbRemove) {

	size_t currIdx = 0;
	while (currIdx < m_size) {

		if (cbRemove(&m_arr[currIdx])) {
			Remove(currIdx);
		}
		currIdx++;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template<typename T>
void BeirusCompactArray<T>::Resize() {

	size_t newCapacity = m_capacity * 3;

	T* tmpArr = (T*)malloc(newCapacity * sizeof(T));
	memcpy_s(tmpArr, newCapacity * sizeof(T), m_arr, m_capacity * sizeof(T));

	for (size_t i = 0; i < m_size; i++) {
		m_arr[i].~T();
	}
	free(m_arr);
	m_arr = tmpArr;
	m_capacity = newCapacity;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GET
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
T& BeirusCompactArray<T>::operator[](int idx) {

	#ifdef __DEBUG
	ASSERT_OR_DIE(idx < m_size, "ERROR: Index out of range on BeirusCompactArray.");
	#endif
	return m_arr[idx];
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
T BeirusCompactArray<T>::operator[](int idx) const {

	#ifdef __DEBUG
	ASSERT_OR_DIE(idx < m_size, "ERROR: Index out of range on BeirusCompactArray.");
	#endif
	return m_arr[idx];
}