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
	
		D3D11ShaderProgram* nShaderProgram = new D3D11ShaderProgram();
		s_shaderProgramRegistry.insert(D3D11ShaderProgramMapPair(hash, nShaderProgram));
		return nShaderProgram;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::Use() {

	GetDeviceContext()->VSSetShader(m_pVertexShader->GetShaderHandle(), nullptr, 0);
	GetDeviceContext()->PSSetShader(m_pPixelShader->GetShaderHandle(), nullptr, 0);

	BindConstantBuffers();
	BindResources();
	BindSamplers();

	//ID3D11Buffer* pConstBufferHandle = m_cBuffer->GetDeviceBufferHandle();
	//ID3D11Buffer* pLightBufferHandle = m_lightBuffer->GetDeviceBufferHandle();
	//ID3D11SamplerState* pSamplerStateHandle = g_samplerState.GetSamplerHandle();
	//
	//GetDeviceContext()->VSSetShader(m_pVertexShader->GetShaderHandle(), nullptr, 0);
	//GetDeviceContext()->VSSetConstantBuffers(0, 1, &pConstBufferHandle);
	//GetDeviceContext()->PSSetShader(m_pPixelShader->GetShaderHandle(), nullptr, 0);
	//GetDeviceContext()->PSSetShaderResources(0, 1, &texID);
	//GetDeviceContext()->PSSetShaderResources(1, 1, &normID);
	//GetDeviceContext()->PSSetConstantBuffers(1, 1, &pLightBufferHandle);
	//GetDeviceContext()->PSSetSamplers(0, 1, &pSamplerStateHandle);
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ShaderProgram::BindConstantBuffers() {

	for (size_t i = 0; i < m_constBuffers.size(); i++) {

		ID3D11Buffer* pConstBufferHandle = m_constBuffers[i].m_pConstBuffer->GetDeviceBufferHandle();
		uint bindPoint = m_constBuffers[i].m_bindPoint;

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