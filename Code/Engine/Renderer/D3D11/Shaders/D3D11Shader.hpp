#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include <d3d11_1.h>

enum eD3D11ShaderType {
	D3D11SHADERTYPE_VERTEX = 0,
	D3D11SHADERTYPE_FRAGMENT,
	D3D11SHADERTYPE_NUM_SHADERTYPES
};

class D3D11Shader {
public:
	static D3D11Shader* CreateOrGetShader(const String& shaderPath, eD3D11ShaderType shaderType);

	virtual ~D3D11Shader();

	ID3DBlob* GetCompiledBlob() { return m_compiledBlob; }

protected:
	D3D11Shader(const char* shaderName, eD3D11ShaderType shaderType);

	ID3D11VertexShader* CreateVertexShader();
	ID3D11PixelShader* CreatePixelShader();

private:
	static String SerializeShaderType(eD3D11ShaderType shaderType);

	String				m_shaderName;
	eD3D11ShaderType	m_shaderType;
	ID3DBlob*			m_compiledBlob;

	static std::map<size_t, D3D11Shader*> s_shaderRegistry;
};

typedef std::map<size_t, D3D11Shader*>				D3D11ShaderMap;
typedef std::map<size_t, D3D11Shader*>::iterator	D3D11ShaderMapIter;
typedef std::pair<size_t, D3D11Shader*>				D3D11ShaderMapPair;