#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"


//---------------------------------------------------------------------------------------------------------------------------
class D3D11Material {
public:
	D3D11Material(const String& shaderName);
	~D3D11Material() { }

	void AddUniform(const String& constBufferName, D3D11Uniform* matUni);

	void AddConstantBuffer(uint bindPoint, D3D11ConstantBuffer* pConstBuffer, eWhichShaderBound whichShadersToBindTo);
	void AddResource(uint bindPoint, D3D11Resource* pResource, eWhichShaderBound whichShadersToBindTo);
	void AddSampler(uint bindPoint, D3D11SamplerState* pSampler, eWhichShaderBound whichShadersToBindTo);

	void Use();

private:
	void BindResources();
	void BindSamplers();

	D3D11ShaderProgram* m_shaderProg = nullptr;

	std::vector<D3D11BufferUniform>		m_uniforms;
	std::vector<ResourceBindInfo>		m_resources;
	std::vector<SamplerBindInfo>		m_samplers;
};


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Material::AddUniform(const String& constBufferName, D3D11Uniform* matUni) {

	D3D11BufferUniform bufferUniform;
	bufferUniform.cBufferName = constBufferName;
	bufferUniform.uniform = matUni;
	m_uniforms.push_back(bufferUniform);
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