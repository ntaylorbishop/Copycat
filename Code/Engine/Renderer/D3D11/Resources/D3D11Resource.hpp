#pragma once

#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"

enum eResourceType {
	RESOURCE_TYPE_DEPTH_STENCIL_VIEW = 0,
	RESOURCE_TYPE_RENDER_TARGET_VIEW,
	RESOURCE_TYPE_SHADER_RESOURCE_VIEW,
	RESOURCE_TYPE_UNORDERED_ACCESS_VIEW,
	RESOURCE_TYPE_NUM_VIEWS
};

class D3D11Resource {
public:
	D3D11Resource() { }
	D3D11Resource(ID3D11Resource* pResource, eResourceType resourceType);
	~D3D11Resource();

	ID3D11DepthStencilView*		AsDepthStencilView();
	ID3D11RenderTargetView*		AsRenderTargetView();
	ID3D11ShaderResourceView*	AsShaderResourceView();
	ID3D11UnorderedAccessView*	AsUnorderedAccessView();
	ID3D11Resource*				GetResource()				{ return m_pResource;		}
	eResourceType				GetResourceType()			{ return m_resourceType;	}

private:
	ID3D11Resource* m_pResource		= nullptr;
	eResourceType	m_resourceType	= RESOURCE_TYPE_NUM_VIEWS;
	ID3D11ShaderResourceView*		m_view			= nullptr;
};

