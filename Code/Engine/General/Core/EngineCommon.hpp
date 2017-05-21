#pragma once

#include <string>
#include <map>
#include <vector>

#include "Engine/General/Core/Memory/UntrackedAllocator.hpp"
#include "Engine/Utils/ErrorWarningAssert.hpp"
#include "Engine/Math/Vector2.hpp"

typedef unsigned char byte;
typedef Vector2 TexCoords;
typedef unsigned int uint;
typedef std::string String;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned short ushort;

typedef byte KEYCODE;


#define GLOBAL
#define STATIC
#define VIRTUAL
#define INLINE
#define LOCAL_STATIC

#define UNUSED(x) x;

#define KB				* 1024;
#define MB				* 1024 * 1024;
#define GB				* 1024 * 1024 * 1024;
#define COMPARE(a, b)	(((a) > (b)) - ((a) < (b)))


const uint	UINT_FALSE	= 0;
const uint	UINT_TRUE	= 1;
const float	FLOAT_ZERO	= 0.f;
const float FLOAT_ONE	= 1.f;

const bool BOOL_TRUE	= true;
const bool BOOL_FALSE	= false;


#define SAFE_DELETE(ptr) if(ptr) { delete ptr; ptr = nullptr; }