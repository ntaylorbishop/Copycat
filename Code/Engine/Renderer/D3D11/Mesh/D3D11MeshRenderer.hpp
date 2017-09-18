#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11ShaderProgram.hpp"

class D3D11Mesh;
class D3D11Material;

class D3D11MeshRenderer {
public:
	D3D11MeshRenderer()		= default;
	~D3D11MeshRenderer()	= default;

	void RenderMeshWithMaterial(D3D11Mesh* mesh, D3D11Material* mat);

private:

	std::vector<D3D11BufferUniform>		m_uniforms;
	std::vector<ResourceBindInfo>		m_resources;
	std::vector<SamplerBindInfo>		m_samplers;
};