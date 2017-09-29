#include "Engine/Renderer/D3D11/General/D3D11Renderer.hpp"
#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/General/D3D11ShaderFileParsing.hpp"
#include "Engine/Renderer/D3D11/Texture/Texture2D.hpp"
#include "Engine/Renderer/D3D11/General/D3D11BlendState.hpp"
#include <d3d11_1.h>


STATIC D3D11Renderer* D3D11Renderer::s_d3d11Renderer;


//---------------------------------------------------------------------------------------------------------------------------
STATIC D3D11Renderer* D3D11Renderer::Get() {

	if (s_d3d11Renderer != nullptr) {
		return s_d3d11Renderer;
	}
	else {
		s_d3d11Renderer = new D3D11Renderer();
		return s_d3d11Renderer;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Renderer::SetViewport(const Vector2& viewportSize) {

	D3D11_VIEWPORT viewport;

	viewport.Width		= (FLOAT)viewportSize.x;
	viewport.Height		= (FLOAT)viewportSize.y;
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	RHIDeviceWindow::Get()->GetD3DContext()->RSSetViewports(1, &viewport);
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Renderer::EnableAlphaBlending() {

	m_defaultAlphaBlendState->BindToDevice();
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Renderer::EnableOpaqueBlending() {

	D3D11BlendState::BindDefault();
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Renderer::D3D11Renderer() {

	Texture2D* defaultDiffuse = new Texture2D("Data/Textures/Brick2.png", true, TEXTURE_BIND_SHADER_RESOURCE, (eTextureCPUAccessFlags)0);
	Texture2D* defaultNormal = new Texture2D("Data/Textures/Brick_Normal.png", true, TEXTURE_BIND_SHADER_RESOURCE, (eTextureCPUAccessFlags)0);
	UNUSED(defaultDiffuse);
	UNUSED(defaultNormal);

	InitializeDefaultBlendStates();
	CreateDefaultSamplers();
	ParseInDefaultConstantBuffers();
	ParseInAllShaderData();
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Renderer::~D3D11Renderer() {

	delete m_defaultAlphaBlendState;
	m_defaultAlphaBlendState = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Renderer::InitializeDefaultBlendStates() {

	m_defaultAlphaBlendState = new D3D11BlendState(false, false, true, BLEND_MODE_SRC_ALPHA, BLEND_MODE_INV_SRC_ALPHA, BLEND_OP_ADD,
		BLEND_MODE_ONE, BLEND_MODE_ZERO, BLEND_OP_ADD, COLOR_WRITE_ENABLE_ALL);
}