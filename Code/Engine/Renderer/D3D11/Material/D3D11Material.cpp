#include "Engine/Renderer/D3D11/Material/D3D11Material.hpp"
#include "Engine/Renderer/D3D11/Texture/Texture2D.hpp"


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Material::Use() {
	
	m_shaderProg->BindShaders();
	m_shaderProg->BindConstantBuffers(m_uniforms);
	m_shaderProg->BindResources();
	m_shaderProg->BindSamplers();

	BindResources();
	BindSamplers();
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Material::BindResources() {

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
void D3D11Material::BindSamplers() {

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
D3D11Uniform* D3D11Material::GetUniform(const String& uniformName) {

	for (size_t i = 0; i < m_uniforms.size(); i++) {

		String matUniName = m_uniforms[i]->GetName();

		if (matUniName == uniformName) {
			return m_uniforms[i];
		}
	}

	return nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Material::ValidateNewUniform(const String& newUniName) {

	bool uniExistsOnShader = false;
	for (size_t i = 0; i < m_shaderProg->m_constBuffers.size(); i++) {

		D3D11ConstantBuffer* currBuffer = m_shaderProg->m_constBuffers[i].m_pConstBuffer;
		for (size_t j = 0; j < currBuffer->m_uniforms.size(); j++) {

			String currBufferUniName = currBuffer->m_uniforms[j]->GetName();
			if (currBufferUniName == newUniName) {
				uniExistsOnShader = true;
			}
		}
	}

	ASSERT_OR_DIE(uniExistsOnShader, "Uniform name of new uniform being added to D3D11Material does not exist on shader");
}