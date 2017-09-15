#include "Engine/Renderer/D3D11/General/D3D11Renderer.hpp"
#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/General/D3D11ShaderFileParsing.hpp"
#include "Engine/Renderer/D3D11/Texture/Texture2D.hpp"
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
D3D11Renderer::D3D11Renderer() {

	Texture2D* defaultDiffuse = new Texture2D("Data/Textures/Brick2.png", true, TEXTURE_BIND_SHADER_RESOURCE, (eTextureCPUAccessFlags)0);
	Texture2D* defaultNormal = new Texture2D("Data/Textures/Brick_Normal.png", true, TEXTURE_BIND_SHADER_RESOURCE, (eTextureCPUAccessFlags)0);
	UNUSED(defaultDiffuse);
	UNUSED(defaultNormal);

	CreateDefaultSamplers();
	ParseInDefaultConstantBuffers();
	ParseInAllShaderData();

	int a = 0;
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Renderer::~D3D11Renderer() {

}

