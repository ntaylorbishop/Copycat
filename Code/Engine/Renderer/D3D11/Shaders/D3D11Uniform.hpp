#pragma once

#include "stddef.h"
#include "Engine/Utils/GeneralUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Renderer/Shaders/Sampler2D.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/Shaders/UniformConstants.hpp"
#include <vector>
#include <string>


class D3D11Texture;
class TextureBuffer;
class TextureCubemap;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UNIFORM BASE CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class D3D11Uniform {
public:
	//STRUCTORS
	D3D11Uniform(const char* name, size_t count, eUniformType type, void* data);
	D3D11Uniform(const char* name, eUniformType type, void* data);
	virtual ~D3D11Uniform() { }


	//GETTERS SETTERS
	static size_t	GetSizeOfType(eUniformType type);
	eUniformType	GetType() const						{ return m_type;		}
	size_t			GetNameHash() const					{ return m_nameHash;	}
	size_t			GetCount() const					{ return m_count;		}
	void*			GetData()							{ return m_data;		}
	const char*		GetName() const						{ return m_name;		}
	size_t			GetByteSize() const					{ return m_elemSize;	}
	void			SetCount(size_t count)				{ m_count	= count;	}
	void			SetData(void* data)					{ m_data	= data;		}

	//TYPE CHECKING
	template <typename T>
	static bool IsCorrectType(eUniformType type, T data) { UNUSED(type); UNUSED(data); return false; }

	template <> static bool IsCorrectType(eUniformType type, float* data)					{ UNUSED(data); return type == UNIFORM_FLOAT;			}
	template <> static bool IsCorrectType(eUniformType type, const float* data)				{ UNUSED(data); return type == UNIFORM_FLOAT;			}
	template <> static bool IsCorrectType(eUniformType type, double* data)					{ UNUSED(data); return type == UNIFORM_DOUBLE;			}
	template <> static bool IsCorrectType(eUniformType type, const double* data)			{ UNUSED(data); return type == UNIFORM_DOUBLE;			}
	template <> static bool IsCorrectType(eUniformType type, Vector3* data)					{ UNUSED(data); return type == UNIFORM_VECTOR3;			}
	template <> static bool IsCorrectType(eUniformType type, const Vector3* data)			{ UNUSED(data); return type == UNIFORM_VECTOR3;			}
	template <> static bool IsCorrectType(eUniformType type, Vector2* data)					{ UNUSED(data); return type == UNIFORM_VECTOR2;			}
	template <> static bool IsCorrectType(eUniformType type, const Vector2* data)			{ UNUSED(data); return type == UNIFORM_VECTOR2;			}
	template <> static bool IsCorrectType(eUniformType type, Vector4* data)					{ UNUSED(data); return type == UNIFORM_VECTOR4;			}
	template <> static bool IsCorrectType(eUniformType type, const Vector4* data)			{ UNUSED(data); return type == UNIFORM_VECTOR4;			}
	template <> static bool IsCorrectType(eUniformType type, Matrix4* data)					{ UNUSED(data); return type == UNIFORM_MAT4;			}
	template <> static bool IsCorrectType(eUniformType type, const Matrix4* data)			{ UNUSED(data); return type == UNIFORM_MAT4;			}
	template <> static bool IsCorrectType(eUniformType type, int* data)						{ UNUSED(data); return type == UNIFORM_INT;				}
	template <> static bool IsCorrectType(eUniformType type, const int* data)				{ UNUSED(data); return type == UNIFORM_INT;				}
	template <> static bool IsCorrectType(eUniformType type, uint* data)					{ UNUSED(data); return type == UNIFORM_UINT;			}
	template <> static bool IsCorrectType(eUniformType type, const uint* data)				{ UNUSED(data); return type == UNIFORM_UINT;			}
	template <> static bool IsCorrectType(eUniformType type, RGBA* data)					{ UNUSED(data); return type == UNIFORM_RGBA;			}
	template <> static bool IsCorrectType(eUniformType type, const RGBA* data)				{ UNUSED(data); return type == UNIFORM_RGBA;			}
	template <> static bool IsCorrectType(eUniformType type, D3D11Texture* data)			{ UNUSED(data); return type == UNIFORM_TEXTURE2D;		}
	template <> static bool IsCorrectType(eUniformType type, const D3D11Texture* data)		{ UNUSED(data); return type == UNIFORM_TEXTURE2D;		}
	template <> static bool IsCorrectType(eUniformType type, TextureBuffer* data)			{ UNUSED(data); return type == UNIFORM_TEXTUREBUFFER2D;	}
	template <> static bool IsCorrectType(eUniformType type, const TextureBuffer* data)		{ UNUSED(data); return type == UNIFORM_TEXTUREBUFFER2D;	}
	template <> static bool IsCorrectType(eUniformType type, TextureCubemap* data)			{ UNUSED(data); return type == UNIFORM_CUBEMAP;			}
	template <> static bool IsCorrectType(eUniformType type, const TextureCubemap* data)	{ UNUSED(data); return type == UNIFORM_CUBEMAP;			}
	template <> static bool IsCorrectType(eUniformType type, bool* data)					{ UNUSED(data); return type == UNIFORM_BOOL;			}
	template <> static bool IsCorrectType(eUniformType type, const bool* data)				{ UNUSED(data); return type == UNIFORM_BOOL;			}

	static String		SerializeType(eUniformType type);
	static eUniformType UnserializeType(const String& typeStr);
	static bool			NeedsBindPoint(eUniformType type);

private:
	char			m_name[256];
	size_t			m_nameHash = 0;
	size_t			m_count = 0;
	size_t			m_elemSize = 0;
	eUniformType	m_type = UNIFORM_NUM_UNIFORM_TYPES;
	void*			m_data = nullptr;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline D3D11Uniform::D3D11Uniform(const char* name, size_t count, eUniformType type, void* data)
	: m_nameHash(HashCString(name))
	, m_count(count)
	, m_elemSize(GetSizeOfType(type))
	, m_type(type)
	, m_data(data)
{
	strcpy_s(m_name, name);
}
inline D3D11Uniform::D3D11Uniform(const char* name, eUniformType type, void* data)
	: m_nameHash(HashCString(name))
	, m_count(1)
	, m_elemSize(GetSizeOfType(type))
	, m_type(type)
	, m_data(data)
{
	strcpy_s(m_name, name);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC inline bool D3D11Uniform::NeedsBindPoint(eUniformType type) {
	return (type == UNIFORM_CUBEMAP || type == UNIFORM_TEXTURE2D || type == UNIFORM_TEXTUREBUFFER2D);
}
