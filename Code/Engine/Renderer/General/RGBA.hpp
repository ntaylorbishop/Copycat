#pragma once

#include "Engine/Utils/StringUtils.hpp"

class RGBA {
public:
	constexpr RGBA()												: r(0.f) , g(0.f) , b(0.f), a(0.f)		{ }
	constexpr RGBA(float rVal, float gVal, float bVal, float aVal)	: r(rVal), g(gVal), b(bVal), a(aVal)	{ }
	RGBA(int rVal, int gVal, int bVal, int aVal)					{ ConvertHex(rVal, gVal, bVal, aVal); }
	RGBA(int rVal, int gVal, int bVal, float aVal)					: a(aVal) { ConvertHex(rVal, gVal, bVal); }

	//Normalizes this color's RGBA's to the given hex values
	void ConvertHex(int rVal, int gVal, int bVal, int aVal);
	void ConvertHex(int rVal, int gVal, int bVal);

	String ToString() const { return std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a); }

	inline bool operator==(const RGBA& other) const;
	inline bool operator!=(const RGBA& other) const;

public:
	float r;
	float g;
	float b;
	float a;

public:
	static const RGBA WHITE;
	static const RGBA BLACK;
	static const RGBA RED;
	static const RGBA GREEN;
	static const RGBA BLUE;
	static const RGBA YELLOW;
	static const RGBA MAGENTA;
	static const RGBA CYAN;
	static const RGBA GREY;
	static const RGBA ORANGE;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
bool RGBA::operator==(const RGBA& other) const {
	return (r == other.r && g == other.g && b == other.b && a == other.a);
}


//---------------------------------------------------------------------------------------------------------------------------
bool RGBA::operator!=(const RGBA& other) const {
	return !(*this == other);
}


