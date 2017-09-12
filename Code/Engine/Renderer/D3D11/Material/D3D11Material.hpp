#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"


class D3D11Material {
public:
	D3D11Material(const String& shaderName);
	~D3D11Material() { }

	void AddConstantBuffer(uint bindPoint, D3D11ConstantBuffer* pConstBuffer, eWhichShaderBound whichShadersToBindTo);
	void AddResource(uint bindPoint, D3D11Resource* pResource, eWhichShaderBound whichShadersToBindTo);
	void AddSampler(uint bindPoint, D3D11SamplerState* pSampler, eWhichShaderBound whichShadersToBindTo);

	void Use();

private:
	void BindConstantBuffers();
	void BindResources();
	void BindSamplers();

	D3D11ShaderProgram* m_shaderProg = nullptr;

	std::vector<ConstBufferBindInfo>	m_constBuffers;
	std::vector<ResourceBindInfo>		m_resources;
	std::vector<SamplerBindInfo>		m_samplers;
};


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Material::AddConstantBuffer(uint bindPoint, D3D11ConstantBuffer* pConstBuffer, eWhichShaderBound whichShadersToBindTo) {

	ConstBufferBindInfo boundBuffer;
	boundBuffer.m_bindPoint = bindPoint;
	boundBuffer.m_pConstBuffer = pConstBuffer;
	boundBuffer.m_whichShaders = whichShadersToBindTo;
	m_constBuffers.push_back(boundBuffer);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Material::AddResource(uint bindPoint, D3D11Resource* pResource, eWhichShaderBound whichShadersToBindTo) {

	ResourceBindInfo boundResource;
	boundResource.m_bindPoint = bindPoint;
	boundResource.m_pResource = pResource;
	boundResource.m_whichShaders = whichShadersToBindTo;
	m_resources.push_back(boundResource);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Material::AddSampler(uint bindPoint, D3D11SamplerState* pSampler, eWhichShaderBound whichShadersToBindTo) {

	SamplerBindInfo boundResource;
	boundResource.m_bindPoint = bindPoint;
	boundResource.m_pSamplerState = pSampler;
	boundResource.m_whichShaders = whichShadersToBindTo;
	m_samplers.push_back(boundResource);
}