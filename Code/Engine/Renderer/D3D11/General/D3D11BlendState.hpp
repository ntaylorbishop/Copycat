#pragma once

#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/General/D3D11Common.hpp"

class ID3D11BlendState;



class D3D11BlendState {
public:
	D3D11BlendState(bool alphaToCoverage, bool independentBlend, bool blendEnabled, eD3D11BlendMode srcBlend, eD3D11BlendMode destBlend,
		eD3D11BlendOp blendOp, eD3D11BlendMode srcBlendAlpha, eD3D11BlendMode destBlendAlpha, eD3D11BlendOp blendOpAlpha, eColorWrite renderTargetWriteMask);
	~D3D11BlendState();

	void BindToDevice();
	static void BindDefault();

private:

	ID3D11BlendState* m_blendState = nullptr;
};