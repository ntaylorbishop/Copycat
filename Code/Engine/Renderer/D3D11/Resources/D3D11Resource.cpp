#include "Engine/Renderer/D3D11/Resources/D3D11Resource.hpp"


//---------------------------------------------------------------------------------------------------------------------------
D3D11Resource::D3D11Resource(ID3D11Resource* pResource, eResourceType resourceType)
	: m_pResource(pResource)
	, m_resourceType(resourceType)
{
	switch (m_resourceType) {
	case RESOURCE_TYPE_DEPTH_STENCIL_VIEW: {

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		HRESULT result = GetDevice()->CreateDepthStencilView(m_pResource, &descDSV, (ID3D11DepthStencilView**)&m_view);
		ASSERT_OR_DIE(SUCCEEDED(result), "Could not create depth stencil view.");
		break;
	}
	case RESOURCE_TYPE_RENDER_TARGET_VIEW: {

		HRESULT result = GetDevice()->CreateRenderTargetView(m_pResource, nullptr, (ID3D11RenderTargetView**)&m_view);
		ASSERT_OR_DIE(SUCCEEDED(result), "Could not create render target view.");
		break;
	}
	case RESOURCE_TYPE_SHADER_RESOURCE_VIEW: {

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.MipLevels = (UINT) - 1;
		srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //NOTE: Only supporting 4-channel 8bit textures for now

		HRESULT result = GetDevice()->CreateShaderResourceView(m_pResource, &srv_desc, &m_view);
		ASSERT_OR_DIE(SUCCEEDED(result), "Could not create shader resource view.");
		break;
	}
	case RESOURCE_TYPE_UNORDERED_ACCESS_VIEW: {

		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;;
		desc.Texture2D.MipSlice = 0;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //NOTE: Only supporting 4-channel 8bit textures for now

		HRESULT result = GetDevice()->CreateUnorderedAccessView(m_pResource, &desc, (ID3D11UnorderedAccessView**)&m_view);
		ASSERT_OR_DIE(SUCCEEDED(result), "Could not create unordered access view.");
		break;
	}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Resource::~D3D11Resource() {

	return;
	m_pResource->Release();
	m_view->Release();
}


//---------------------------------------------------------------------------------------------------------------------------
ID3D11DepthStencilView* D3D11Resource::AsDepthStencilView() {

	ASSERT_OR_DIE(m_resourceType == RESOURCE_TYPE_DEPTH_STENCIL_VIEW, "ERROR: Resource is not depth stencil view.");
	return (ID3D11DepthStencilView*)m_view;
}


//---------------------------------------------------------------------------------------------------------------------------
ID3D11RenderTargetView* D3D11Resource::AsRenderTargetView() {

	ASSERT_OR_DIE(m_resourceType == RESOURCE_TYPE_RENDER_TARGET_VIEW, "ERROR: Resource is not depth stencil view.");
	return (ID3D11RenderTargetView*)m_view;
}


//---------------------------------------------------------------------------------------------------------------------------
ID3D11ShaderResourceView* D3D11Resource::AsShaderResourceView() {

	//WARNING: Commenting out because of memory corruption on these resources
	//ASSERT_OR_DIE(m_resourceType == RESOURCE_TYPE_SHADER_RESOURCE_VIEW, "ERROR: Resource is not depth stencil view.");
	return (ID3D11ShaderResourceView*)m_view;
}


//---------------------------------------------------------------------------------------------------------------------------
ID3D11UnorderedAccessView* D3D11Resource::AsUnorderedAccessView() {

	ASSERT_OR_DIE(m_resourceType == RESOURCE_TYPE_UNORDERED_ACCESS_VIEW, "ERROR: Resource is not depth stencil view.");
	return (ID3D11UnorderedAccessView*)m_view;
}