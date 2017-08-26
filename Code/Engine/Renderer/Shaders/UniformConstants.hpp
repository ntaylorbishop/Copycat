#pragma once

const int	INVALID_SHADER_LOC		= -1;
const short MAX_UNIFORM_NAME_SIZE	= 128;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UNIFORM TYPE ENUM
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum eUniformType {
	UNIFORM_FLOAT = 0,
	UNIFORM_DOUBLE,
	UNIFORM_VECTOR3,
	UNIFORM_VECTOR2,
	UNIFORM_VECTOR4,
	UNIFORM_MAT4,
	UNIFORM_INT,
	UNIFORM_UINT,
	UNIFORM_SHORT,
	UNIFORM_RGBA,
	UNIFORM_TEXTURE2D,
	UNIFORM_TEXTUREBUFFER2D,
	UNIFORM_CUBEMAP,
	UNIFORM_BOOL,
	UNIFORM_NUM_UNIFORM_TYPES
};