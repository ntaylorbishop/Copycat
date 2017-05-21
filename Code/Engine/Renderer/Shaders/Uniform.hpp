#pragma once

#include "stddef.h"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Renderer/Shaders/Sampler2D.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include <vector>
#include <string>
#include "Engine/Utils/GeneralUtils.hpp"

//NOTE: Any sampler types cannot be made arrays.

const int INVALID_SHADER_LOC = -1;
const short MAX_UNIFORM_NAME_SIZE = 128;

class Texture;
class TextureBuffer;
class TextureCubemap;

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UNIFORM BASE CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Uniform {
public:
	//STRUCTORS
	Uniform(const char* name, size_t count, eUniformType type, int locInShader, int bindPoint, void* data);
	Uniform(const char* name, eUniformType type, int locInShader, int bindPoint, void* data);
	virtual ~Uniform() { }


	//GETTERS SETTERS
	static size_t	GetSizeOfType(eUniformType type);
	eUniformType	GetType() const					{ return m_type;					}
	int				GetLocInShader() const			{ return m_locInShader;				}
	size_t			GetNameHash() const				{ return m_nameHash;				}
	size_t			GetCount() const				{ return m_count;					}
	int				GetBindPoint() const			{ return m_bindPoint;				}
	void*			GetData()						{ return m_data;					}
	const char*		GetName() const					{ return m_name;					}

	void			SetData(void* data)				{ m_data = data; m_isDirty = true;	}
	void			SetCount(size_t count)			{ m_count = count;					}
	void			SetLocInShader(int shaderLoc)	{ m_locInShader = shaderLoc;		}

	//BINDING
	void Bind() const;

	//TYPE CHECKING
	template <typename T>
	static bool IsCorrectType(eUniformType type, T data) { UNUSED(type); UNUSED(data); return false; }

	template <> static bool IsCorrectType(eUniformType type, float* data)				{ UNUSED(data); return type == UNIFORM_FLOAT;			}
	template <> static bool IsCorrectType(eUniformType type, const float* data)			{ UNUSED(data); return type == UNIFORM_FLOAT;			}
	template <> static bool IsCorrectType(eUniformType type, double* data)				{ UNUSED(data); return type == UNIFORM_DOUBLE;			}
	template <> static bool IsCorrectType(eUniformType type, const double* data)		{ UNUSED(data); return type == UNIFORM_DOUBLE;			}
	template <> static bool IsCorrectType(eUniformType type, Vector3* data)				{ UNUSED(data); return type == UNIFORM_VECTOR3;			}
	template <> static bool IsCorrectType(eUniformType type, const Vector3* data)		{ UNUSED(data); return type == UNIFORM_VECTOR3;			}
	template <> static bool IsCorrectType(eUniformType type, Vector2* data)				{ UNUSED(data); return type == UNIFORM_VECTOR2;			}
	template <> static bool IsCorrectType(eUniformType type, const Vector2* data)		{ UNUSED(data); return type == UNIFORM_VECTOR2;			}
	template <> static bool IsCorrectType(eUniformType type, Vector4* data)				{ UNUSED(data); return type == UNIFORM_VECTOR4;			}
	template <> static bool IsCorrectType(eUniformType type, const Vector4* data)		{ UNUSED(data); return type == UNIFORM_VECTOR4;			}
	template <> static bool IsCorrectType(eUniformType type, Matrix4* data)				{ UNUSED(data); return type == UNIFORM_MAT4;			}
	template <> static bool IsCorrectType(eUniformType type, const Matrix4* data)		{ UNUSED(data); return type == UNIFORM_MAT4;			}
	template <> static bool IsCorrectType(eUniformType type, int* data)					{ UNUSED(data); return type == UNIFORM_INT;				}
	template <> static bool IsCorrectType(eUniformType type, const int* data)			{ UNUSED(data); return type == UNIFORM_INT;				}
	template <> static bool IsCorrectType(eUniformType type, uint* data)				{ UNUSED(data); return type == UNIFORM_UINT;			}
	template <> static bool IsCorrectType(eUniformType type, const uint* data)			{ UNUSED(data); return type == UNIFORM_UINT;			}
	template <> static bool IsCorrectType(eUniformType type, RGBA* data)				{ UNUSED(data); return type == UNIFORM_RGBA;			}
	template <> static bool IsCorrectType(eUniformType type, const RGBA* data)			{ UNUSED(data); return type == UNIFORM_RGBA;			}
	template <> static bool IsCorrectType(eUniformType type, Texture* data)				{ UNUSED(data); return type == UNIFORM_TEXTURE2D;		}
	template <> static bool IsCorrectType(eUniformType type, const Texture* data)		{ UNUSED(data); return type == UNIFORM_TEXTURE2D;		}
	template <> static bool IsCorrectType(eUniformType type, TextureBuffer* data)		{ UNUSED(data); return type == UNIFORM_TEXTUREBUFFER2D; }
	template <> static bool IsCorrectType(eUniformType type, const TextureBuffer* data)	{ UNUSED(data); return type == UNIFORM_TEXTUREBUFFER2D; }
	template <> static bool IsCorrectType(eUniformType type, TextureCubemap* data)		{ UNUSED(data); return type == UNIFORM_CUBEMAP;			}
	template <> static bool IsCorrectType(eUniformType type, const TextureCubemap* data){ UNUSED(data); return type == UNIFORM_CUBEMAP;			}
	template <> static bool IsCorrectType(eUniformType type, bool* data)				{ UNUSED(data); return type == UNIFORM_BOOL;			}
	template <> static bool IsCorrectType(eUniformType type, const bool* data)			{ UNUSED(data); return type == UNIFORM_BOOL;			}

	static String		SerializeType(eUniformType type);
	static eUniformType UnserializeType(const String& typeStr);
	static bool			NeedsBindPoint(eUniformType type);

private:
	char		m_name[MAX_UNIFORM_NAME_SIZE];
	size_t			m_nameHash		= 0;
	size_t			m_count			= 0;
	size_t			m_elemSize		= 0;
	eUniformType	m_type			= UNIFORM_NUM_UNIFORM_TYPES;
	int				m_locInShader	= -1;
	void*			m_data			= nullptr;
	mutable bool	m_isDirty		= false;
	int				m_bindPoint		= -1;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
inline Uniform::Uniform(const char* name, size_t count, eUniformType type, int locInShader, int bindPoint, void* data)
	: m_nameHash(HashCString(name))
	, m_count(count)
	, m_elemSize(GetSizeOfType(type))
	, m_type(type)
	, m_locInShader(locInShader)
	, m_data(data)
	, m_isDirty(true)
	, m_bindPoint(bindPoint)
{
	strcpy_s(m_name, name);
	bool invalidBindPoint = (NeedsBindPoint(type) && bindPoint == -1);
	ASSERT_OR_DIE(!invalidBindPoint, "ERROR: Need bind point for texture uniforms.");
}
inline Uniform::Uniform(const char* name, eUniformType type, int locInShader, int bindPoint, void* data)
	: m_nameHash(HashCString(name))
	, m_count(1)
	, m_elemSize(GetSizeOfType(type))
	, m_type(type)
	, m_locInShader(locInShader)
	, m_data(data)
	, m_isDirty(true)
	, m_bindPoint(bindPoint)
{ 
	strcpy_s(m_name, name);
	bool invalidBindPoint = (NeedsBindPoint(type) && bindPoint == -1);
	ASSERT_OR_DIE(!invalidBindPoint, "ERROR: Need bind point for texture uniforms.");
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC inline bool Uniform::NeedsBindPoint(eUniformType type) { 
	return (type == UNIFORM_CUBEMAP || type == UNIFORM_TEXTURE2D || type == UNIFORM_TEXTUREBUFFER2D); 
}
