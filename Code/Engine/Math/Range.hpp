#pragma once

#include "Engine/Math/MathUtils.hpp"

template <typename T>
class Range {
public:
	//STRUCTORS
	Range()											{ }
	Range(T val)		: m_min(val), m_max(val)	{ }
	Range(T min, T max) : m_min(min), m_max(max)	{ }

	T Roll();

private:
	T m_min;
	T m_max;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Range<T>::Roll() {
	return RollBase(m_min, m_max);
}