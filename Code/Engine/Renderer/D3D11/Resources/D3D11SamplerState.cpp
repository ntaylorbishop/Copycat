#include "Engine/Renderer/D3D11/Resources/D3D11SamplerState.hpp"


STATIC std::map<size_t, D3D11SamplerState*> D3D11SamplerState::s_samplerRegistry;


//---------------------------------------------------------------------------------------------------------------------------
D3D11SamplerState::D3D11SamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE uMode, D3D11_TEXTURE_ADDRESS_MODE vMode,
	D3D11_TEXTURE_ADDRESS_MODE wMode, D3D11_COMPARISON_FUNC comparisonFunc) 
{

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = filter;
	sampDesc.AddressU = uMode;
	sampDesc.AddressV = vMode;
	sampDesc.AddressW = wMode;
	sampDesc.ComparisonFunc = comparisonFunc;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = GetDevice()->CreateSamplerState(&sampDesc, &m_pSamplerHandle);
	ASSERT_OR_DIE(SUCCEEDED(hr), "ERROR: Could not create D3D11 sampler state.");
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void D3D11SamplerState::AddSamplerToRegistry(D3D11SamplerState* sampler, const String& samplerName) {

	size_t hash = std::hash<String>()(samplerName);
	D3D11SamplerMapIter it = s_samplerRegistry.find(hash);

	ASSERT_OR_DIE(it == s_samplerRegistry.end(), "ERROR: Sampler already exists in registry with that name.");
	s_samplerRegistry.insert(D3D11SamplerMapPair(hash, sampler));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC D3D11SamplerState* D3D11SamplerState::GetSampler(const String& samplerName) {

	size_t hash = std::hash<String>()(samplerName);
	D3D11SamplerMapIter it = s_samplerRegistry.find(hash);

	if (it != s_samplerRegistry.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}