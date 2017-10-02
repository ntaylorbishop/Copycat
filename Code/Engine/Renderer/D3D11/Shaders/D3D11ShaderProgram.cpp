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
	m_pVertexShader->BindVertexLayout();
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
void D3D11ShaderProgram::BindConstantBuffers(const std::vector<D3D11Uniform*>& matUniforms) {

	//Update buffers and bind them
	for (size_t i = 0; i < m_constBuffers.size(); i++) {

		ID3D11Buffer* pConstBufferHandle = m_constBuffers[i].m_pConstBuffer->GetDeviceBufferHandle();
		uint bindPoint = m_constBuffers[i].m_bindPoint;

		m_constBuffers[i].m_pConstBuffer->UpdateBufferOnDevice(matUniforms);

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


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::AddConstantBuffer(uint bindPoint, D3D11ConstantBuffer* pConstBuffer, eWhichShaderBound whichShadersToBindTo) {

	//Check to make sure that no uniforms share the same name across constant buffers
	for (size_t i = 0; i < pConstBuffer->m_uniforms.size(); i++) {
		
		for (size_t j = 0; j < m_constBuffers.size(); j++) {
		
			D3D11ConstantBuffer* currBuffer = m_constBuffers[j].m_pConstBuffer;
			for (size_t k = 0; k < currBuffer->m_uniforms.size(); k++) {

				String addingBufferUni = pConstBuffer->m_uniforms[i]->GetName();
				String currBufferUni = currBuffer->m_uniforms[k]->GetName();
				ASSERT_OR_DIE(addingBufferUni != currBufferUni, "ERROR: Two constant buffers in a shader program cannot share the same name.");
			}
		}
	}

	ConstBufferBindInfo boundBuffer;
	boundBuffer.m_bindPoint = bindPoint;
	boundBuffer.m_pConstBuffer = pConstBuffer;
	boundBuffer.m_whichShaders = whichShadersToBindTo;
	m_constBuffers.push_back(boundBuffer);
}