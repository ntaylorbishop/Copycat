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
#include "Engine/Renderer/General/Texture.hpp"

enum eTextureFormat;

class TextureBuffer {
public:
	//STRUCTORS
	TextureBuffer(int width, int height, eTextureFormat format);
	~TextureBuffer();

	//GETTERS SETTERS
	int				GetTextureID() const	{ return m_textureID;	}
	int				GetHeight() const		{ return m_texelSize.y; }
	int				GetWidth() const		{ return m_texelSize.x; }
	IntVector2		GetSize() const			{ return m_texelSize;	}
	eTextureFormat	GetFormat() const		{ return m_format;		}
	int				GetSamplerID() const	{ return m_samplerID;	}

	void SetToMaxLOD();
	void SetToMinLOD();

	Texture* AsTexture();


	//BINDING
	void BindToShaderProgram(int locInShader, int bindPoint);
	void GenerateMipmap() const;

private:

	int				m_textureID;
	IntVector2		m_texelSize;
	eTextureFormat	m_format;
	int				m_samplerID;
};
