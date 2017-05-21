#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Core/Memory/UntrackedAllocator.hpp"
#include "Engine/Renderer/OpenGL/OpenGLExtensions.hpp"
#include "Engine/Math/IntVector2.hpp"

enum eTextureFormat;
class Texture;
class TextureBuffer;

const uint CUBEMAP_DEFAULT_PIXEL_FORMAT = 4;	//Assume GL_RGBA for now
												//Also assume that cubemap textures are of uniform size

class TextureCubemap {
public:
	//STRUCTORS
	TextureCubemap(const IntVector2& size, eTextureFormat format);
	TextureCubemap(const char* xPositive, const char* xNegative,
		const char* yPositive, const char* yNegative,
		const char* zPositive, const char* zNegative);

	~TextureCubemap();

	//GETTERS SETTERS
	int GetTextureID() const			{ return m_textureID;	}
	int GetSamplerID() const			{ return m_samplerID;	}
	IntVector2 GetSize() const			{ return m_textureSize; }

	//MODIFIERS
	void GenerateMipmap() const;

	//BINDING
	void BindToShaderProgram(int locInShader, int bindPoint);

	void WriteToFile(const String& filepath, const String& namePrefix);

private:

	uint		m_textureID		= 0;
	uint		m_samplerID		= 0;
	IntVector2	m_textureSize	= IntVector2(0, 0);

	#if defined(_DEBUG)
	eTextureFormat m_format;
	#endif
};
