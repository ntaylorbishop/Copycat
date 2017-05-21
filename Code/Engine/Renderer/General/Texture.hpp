#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <string>
#include <map>
#include <functional>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Core/Memory/UntrackedAllocator.hpp"
#include "Engine/Renderer/OpenGL/OpenGLExtensions.hpp"
#include "Engine/Math/IntVector2.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum eTextureFormat {
	TEXTURE_FORMAT_RGBA8,
	TEXTURE_FORMAT_D24S8,
	TEXTURE_FORMAT_D16,
	TEXTURE_FORMAT_R32F,
	TEXTURE_FORMAT_RGBA16F
};


//---------------------------------------------------------------------------------------------------------------------------
enum eTextureSampleType {
	TEXTURE_SAMPLE_TYPE_CLAMP = 0,
	TEXTURE_SAMPLE_TYPE_WRAP
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEXTURE CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Texture {
public:
	//CREATE AND DEINIT
	static Texture* CreateOrGetTexture(const String& imageFilePath, bool readInLinearSpace = false);
	~Texture();

	static void DeinitTextures();

	//MODIFIERS
	void GenerateMipmap();

	//BINDING
	void BindToShaderProgram(int locInShader, int bindPoint);

	//GETTERS SETTERS
	int		GetTextureID() const		{ return m_openglTextureID;			}
	int		GetHeight() const			{ return m_texelSize.y;				}
	int		GetWidth() const			{ return m_texelSize.x;				}
	String	GetName() const				{ return m_name;					}
	int		GetSamplerID() const		{ return m_samplerID;				}
	bool	IsGammaCorrected() const	{ return m_needsGammaCorrection;	}
	void	SetName(const String& name)	{ m_name = name;					}
	
private:
	//STRUCTORS
	Texture() { }
	Texture(const String& imageFilePath, bool readInLinearSpace = false);


	static std::map<size_t, Texture*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, Texture*>>> s_textureRegistry;

	GLuint		m_openglTextureID	= 0;
	String		m_name				= "NULL";
	IntVector2	m_texelSize			= IntVector2::ZERO;
	bool		m_needsGammaCorrection	= false;
	int			m_samplerID			= 0;

	friend class TextureBuffer;
};

////////////////////////////////////////////////////
//------------------------------------------
//TYPEDEFS
typedef std::map<size_t, Texture*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, Texture*>>> TextureMap;
typedef std::pair<size_t, Texture*> TextureMapPair;
typedef std::map<size_t, Texture*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, Texture*>>>::iterator TextureMapIterator;
typedef std::hash<std::string> StringHash;