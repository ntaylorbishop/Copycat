#pragma once

#include "Engine/General.hpp"
#include "Engine/Renderer/General/Texture.hpp"

class TextureBuffer;

class Framebuffer {
public:

	//STRUCTORS INITIALIZATION
	Framebuffer(std::vector<TextureBuffer*> colors, TextureBuffer* depthStencil, bool gammaCorrect = false);

	//BIND COPY
	void		Bind();
	static void BindBackBuffer();
	void		CopyToBack();

	//GETTERS SETTERS
	int GetPixelWidth() const				{ return m_pixelWidth;			}
	int GetPixelHeight() const				{ return m_pixelHeight;			}
	uint GetFBOHandle() const				{ return m_fboHandle;			}
	size_t GetColorCount() const			{ return m_colorTargets.size(); }
	TextureBuffer* GetColorTexture() const	{ return m_colorTargets[0];		}
	TextureBuffer* GetDepthTexture() const	{ return m_depthStencilTarget;	}

private:

	uint32_t					m_fboHandle				= 0;
	TextureBuffer*				m_depthStencilTarget	= nullptr;
	int							m_pixelWidth			= 0;
	int							m_pixelHeight			= 0;
	bool						m_shouldGammaCorrect	= false;
	std::vector<TextureBuffer*> m_colorTargets;
};