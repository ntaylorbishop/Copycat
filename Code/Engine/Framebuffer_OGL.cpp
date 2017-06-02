#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS INITIALIZATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Framebuffer::Framebuffer(std::vector<TextureBuffer*> renderTargets, TextureBuffer* depthStencil, bool shouldGammaCorrect)
	: m_colorTargets(renderTargets)
	, m_depthStencilTarget(depthStencil)
	, m_shouldGammaCorrect(shouldGammaCorrect)
{

	ASSERT_OR_DIE(!renderTargets.empty() && renderTargets[0] != nullptr, "ERROR: Must supply well-formed texture buffers to FBO");
	m_pixelWidth	= m_colorTargets[0]->GetWidth();
	m_pixelHeight	= m_colorTargets[0]->GetHeight();


	GL_CHECK(glGenFramebuffers(1, &m_fboHandle));
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle));

	for (uint32_t i = 0; i < m_colorTargets.size(); i++) {
		TextureBuffer* tex = m_colorTargets[i];
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex->GetTextureID(), 0));
	}

	if (nullptr != m_depthStencilTarget) {
		GL_CHECK(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_depthStencilTarget->GetTextureID(), 0));
	}

	GLenum status;
	GL_CHECK(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		GL_CHECK(glDeleteFramebuffers(1, &m_fboHandle));
	}

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BIND COPY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Framebuffer::Bind() {

	//if (m_shouldGammaCorrect) {
	//	GL_CHECK(glEnable(GL_FRAMEBUFFER_SRGB));
	//}
	//else {
	//	GL_CHECK(glDisable(GL_FRAMEBUFFER_SRGB));
	//}

	GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, NULL));
	GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboHandle));


	#if IS_WINDOWED
	GL_CHECK(glViewport(0, 0, (GLsizei)1600.f, (GLsizei)900.f));
	#else
	GL_CHECK(glViewport(0, 0, (GLsizei)3840.f, (GLsizei)2160.f));
	#endif

	GLenum renderTargets[32];
	for (unsigned i = 0; i < GetColorCount(); i++) {
		renderTargets[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	GL_CHECK(glDrawBuffers((GLsizei)GetColorCount(), renderTargets));
}


//---------------------------------------------------------------------------------------------------------------------------
void Framebuffer::BindBackBuffer() {

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	GL_CHECK(glDrawBuffer(GL_BACK));

	#if IS_WINDOWED
	GL_CHECK(glViewport(0, 0, 1600, 900));
	#else
	GL_CHECK(glViewport(0, 0, 3840, 2160));
	#endif
}


//---------------------------------------------------------------------------------------------------------------------------
void Framebuffer::CopyToBack() {

	GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboHandle));
	GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NULL));

	#if IS_WINDOWED
	int readWidth	= 1600;
	int readHeight	= 900;
	uint drawWidth	= 1600;
	uint drawHeight = 900;
	#else
	int readWidth	= 3840;
	int readHeight	= 2160;
	int drawWidth	= 3840;
	int drawHeight	= 2160;
	#endif
	GL_CHECK(glBlitFramebuffer(0, 0, readWidth, readHeight, 0, 0, drawWidth, drawHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR));
}

#endif