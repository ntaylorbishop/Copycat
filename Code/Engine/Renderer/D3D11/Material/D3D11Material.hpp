#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"
#include "Engine/Renderer/D3d11/Texture/Texture2D.hpp"

//---------------------------------------------------------------------------------------------------------------------------
class D3D11Material {
public:
	D3D11Material(const String& shaderName);
	~D3D11Material() { }

	void AddUniform(D3D11Uniform* matUni);
	void CreateUniform(const String& name, eUniformType type, void* data);

	void AddConstantBuffer(uint bindPoint, D3D11ConstantBuffer* pConstBuffer, eWhichShaderBound whichShadersToBindTo);
	void AddResource(uint bindPoint, D3D11Resource* pResource, eWhichShaderBound whichShadersToBindTo);
	void AddTextureResource(uint bindPoint, Texture2D* pTexture, eWhichShaderBound whichShadersToBindTo);
	void AddSampler(uint bindPoint, D3D11SamplerState* pSampler, eWhichShaderBound whichShadersToBindTo);

	D3D11Uniform* GetUniform(const String& uniformName);

	void Use();

	void SetName(const String& matName)		{ m_name = matName; }
	String GetName()						{ return m_name;	}

private:
	void BindResources();
	void BindSamplers();
	void ValidateNewUniform(const String& newUniName);


	D3D11ShaderProgram* m_shaderProg	= nullptr;
	String				m_name			= "NULL";

	std::vector<D3D11Uniform*>		m_uniforms;
	std::vector<ResourceBindInfo>	m_resources;
	std::vector<SamplerBindInfo>	m_samplers;
};


//---------------------------------------------------------------------------------------------------------------------------
inline D3D11Material::D3D11Material(const String& shaderName) {

	m_shaderProg = D3D11ShaderProgram::CreateOrGetShaderProgram(shaderName);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Material::CreateUniform(const String& name, eUniformType type, void* data) {

#if _DEBUG
	ValidateNewUniform(name);
#endif

	D3D11Uniform* newUni = new D3D11Uniform(name.c_str(), type, data);
	m_uniforms.push_back(newUni);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Material::AddUniform(D3D11Uniform* matUni) {

#if _DEBUG
	ValidateNewUniform(matUni->GetName());
#endif

	m_uniforms.push_back(matUni);
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


//---------------------------------------------------------------------------------------------------------------------------
inline void D3D11Material::AddTextureResource(uint bindPoint, Texture2D* pTexture, eWhichShaderBound whichShadersToBindTo) {

	D3D11Resource* texID = pTexture->GetSRVResource();
	AddResource(bindPoint, texID, whichShadersToBindTo);
}