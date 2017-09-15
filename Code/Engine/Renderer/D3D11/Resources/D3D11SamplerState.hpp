#pragma once

#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"


class D3D11SamplerState {
public:
	D3D11SamplerState() = default;
	D3D11SamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE uMode, D3D11_TEXTURE_ADDRESS_MODE vMode, 
		D3D11_TEXTURE_ADDRESS_MODE wMode, D3D11_COMPARISON_FUNC comparisonFunc);
	~D3D11SamplerState() = default;

	ID3D11SamplerState* GetSamplerHandle() { return m_pSamplerHandle; }


	static void AddSamplerToRegistry(D3D11SamplerState* sampler, const String& samplerName);
	static D3D11SamplerState* GetSampler(const String& samplerName);

private:
	ID3D11SamplerState*	m_pSamplerHandle = nullptr;

	static std::map<size_t, D3D11SamplerState*> s_samplerRegistry;
};

typedef std::map<size_t, D3D11SamplerState*>			D3D11SamplerMap;
typedef std::map<size_t, D3D11SamplerState*>::iterator	D3D11SamplerMapIter;
typedef std::pair<size_t, D3D11SamplerState*>			D3D11SamplerMapPair;