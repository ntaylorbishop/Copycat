#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"


STATIC std::map<size_t, D3D11ShaderProgram*> D3D11ShaderProgram::s_shaderProgramRegistry;


//---------------------------------------------------------------------------------------------------------------------------
STATIC D3D11ShaderProgram* D3D11ShaderProgram::CreateOrGetShaderProgram(const String& name) {

	size_t hash = std::hash<String>()(name);
	D3D11ShaderProgramMapIter it = s_shaderProgramRegistry.find(hash);

	if (it != s_shaderProgramRegistry.end()) {
		return it->second;
	}
	else {
	
		D3D11ShaderProgram* nShaderProgram = new D3D11ShaderProgram(name);
		s_shaderProgramRegistry.insert(D3D11ShaderProgramMapPair(hash, nShaderProgram));
		return nShaderProgram;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::Use() {

	BindConstantBuffers();
	BindResources();
	BindSamplers();
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::BindShaders() {
	GetDeviceContext()->VSSetShader(m_pVertexShader->GetShaderHandle(), nullptr, 0);
	GetDeviceContext()->PSSetShader(m_pPixelShader->GetShaderHandle(), nullptr, 0);
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::BindConstantBuffers() {

	for (size_t i = 0; i < m_constBuffers.size(); i++) {

		ID3D11Buffer* pConstBufferHandle = m_constBuffers[i].m_pConstBuffer->GetDeviceBufferHandle();
		uint bindPoint = m_constBuffers[i].m_bindPoint;

		m_constBuffers[i].m_pConstBuffer->UpdateBufferOnDevice();

		switch (m_constBuffers[i].m_whichShaders) {
		case WHICH_SHADER_VERTEX: {
			GetDeviceContext()->VSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			break;
		}
		case WHICH_SHADER_FRAGMENT: {
			GetDeviceContext()->PSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			break;
		}
		case WHICH_SHADER_BOTH: {
			GetDeviceContext()->VSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			GetDeviceContext()->PSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			break;
		}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::BindConstantBuffers(const std::vector<D3D11BufferUniform>& matUniforms) {

	//Go through buffers and check if we need to bind new uniforms


	//Update buffers and bind them
	for (size_t i = 0; i < m_constBuffers.size(); i++) {

		ID3D11Buffer* pConstBufferHandle = m_constBuffers[i].m_pConstBuffer->GetDeviceBufferHandle();
		uint bindPoint = m_constBuffers[i].m_bindPoint;

		m_constBuffers[i].m_pConstBuffer->UpdateBufferOnDevice();

		switch (m_constBuffers[i].m_whichShaders) {
		case WHICH_SHADER_VERTEX: {
			GetDeviceContext()->VSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			break;
		}
		case WHICH_SHADER_FRAGMENT: {
			GetDeviceContext()->PSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			break;
		}
		case WHICH_SHADER_BOTH: {
			GetDeviceContext()->VSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			GetDeviceContext()->PSSetConstantBuffers(bindPoint, 1, &pConstBufferHandle);
			break;
		}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::BindResources() {

	for (size_t i = 0; i < m_resources.size(); i++) {

		ID3D11ShaderResourceView* pResourceHandle = m_resources[i].m_pResource->AsShaderResourceView();
		uint bindPoint = m_resources[i].m_bindPoint;

		switch (m_resources[i].m_whichShaders) {
		case WHICH_SHADER_VERTEX: {
			GetDeviceContext()->VSSetShaderResources(bindPoint, 1, &pResourceHandle);
			break;
		}
		case WHICH_SHADER_FRAGMENT: {
			GetDeviceContext()->PSSetShaderResources(bindPoint, 1, &pResourceHandle);
			break;
		}
		case WHICH_SHADER_BOTH: {
			GetDeviceContext()->VSSetShaderResources(bindPoint, 1, &pResourceHandle);
			GetDeviceContext()->PSSetShaderResources(bindPoint, 1, &pResourceHandle);
			break;
		}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::BindSamplers() {

	for (size_t i = 0; i < m_samplers.size(); i++) {

		ID3D11SamplerState* pSamplerHandle = m_samplers[i].m_pSamplerState->GetSamplerHandle();
		uint bindPoint = m_samplers[i].m_bindPoint;

		switch (m_samplers[i].m_whichShaders) {
		case WHICH_SHADER_VERTEX: {
			GetDeviceContext()->VSSetSamplers(bindPoint, 1, &pSamplerHandle);
			break;
		}
		case WHICH_SHADER_FRAGMENT: {
			GetDeviceContext()->PSSetSamplers(bindPoint, 1, &pSamplerHandle);
			break;
		}
		case WHICH_SHADER_BOTH: {
			GetDeviceContext()->VSSetSamplers(bindPoint, 1, &pSamplerHandle);
			GetDeviceContext()->PSSetSamplers(bindPoint, 1, &pSamplerHandle);
			break;
		}
		}
	}
}