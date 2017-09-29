#include "Engine/Renderer/D3D11/General/D3D11BlendState.hpp"

D3D11BlendState::D3D11BlendState(bool alphaToCoverage, bool independentBlend, bool blendEnabled, eD3D11BlendMode srcBlend, eD3D11BlendMode destBlend,
	eD3D11BlendOp blendOp, eD3D11BlendMode srcBlendAlpha, eD3D11BlendMode destBlendAlpha, eD3D11BlendOp blendOpAlpha, eColorWrite renderTargetWriteMask)
{
	
	D3D11_BLEND_DESC blendState;
	ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));

	blendState.AlphaToCoverageEnable = alphaToCoverage;
	blendState.IndependentBlendEnable = independentBlend;

	blendState.RenderTarget[0].BlendEnable				= blendEnabled;
	blendState.RenderTarget[0].SrcBlend					= (D3D11_BLEND)srcBlend;
	blendState.RenderTarget[0].DestBlend				= (D3D11_BLEND)destBlend;
	blendState.RenderTarget[0].BlendOp					= (D3D11_BLEND_OP)blendOp;
	blendState.RenderTarget[0].SrcBlendAlpha			= (D3D11_BLEND)srcBlendAlpha;
	blendState.RenderTarget[0].DestBlendAlpha			= (D3D11_BLEND)destBlendAlpha;
	blendState.RenderTarget[0].BlendOpAlpha				= (D3D11_BLEND_OP)blendOpAlpha;
	blendState.RenderTarget[0].RenderTargetWriteMask	= (UINT8)renderTargetWriteMask;

	GetDevice()->CreateBlendState(&blendState, &m_blendState);
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11BlendState::~D3D11BlendState() {

	m_blendState->Release();
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11BlendState::BindToDevice() {

	GetDeviceContext()->OMSetBlendState(m_blendState, 0, 0xffffffff);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void D3D11BlendState::BindDefault() {

	GetDeviceContext()->OMSetBlendState(NULL, 0, 0xffffffff);
}