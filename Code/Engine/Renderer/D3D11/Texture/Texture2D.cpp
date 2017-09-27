#include "Engine/Renderer/D3D11/Texture/Texture2D.hpp"

#define STBI_HEADER_FILE_ONLY

#pragma warning(push)
#include "ThirdParty/stb_image.c"
#pragma warning(pop)


STATIC D3D11TextureMap Texture2D::s_textureRegistry;


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Texture2D::ShutdownEvent(NamedProperties& np) {

	D3D11TextureMapIter it = s_textureRegistry.begin();

	for (it; it != s_textureRegistry.end(); ++it) {
		delete it->second;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Texture2D* Texture2D::GetTexture(const char* imageFilePath) {

	size_t hash = std::hash<String>()(String(imageFilePath));
	D3D11TextureMapIter it = s_textureRegistry.find(hash);

	if (it != s_textureRegistry.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Texture2D::Texture2D(const char* imageFilePath, bool generateMips, eTextureBindFlags bindFlags, eTextureCPUAccessFlags accessFlags) {

	if (s_textureRegistry.empty()) {
		EventSystem::RegisterEventCallback("Shutdown", &Texture2D::ShutdownEvent);
	}

	UNREFERENCED_PARAMETER(accessFlags);
	UNREFERENCED_PARAMETER(bindFlags);

	m_resourceViews.resize(RESOURCE_TYPE_NUM_VIEWS);

	int numComponents = 0;
	int numComponentsRequested = 0;

	unsigned char* imageData = stbi_load(imageFilePath, &m_textureSize.x, &m_textureSize.y, &numComponents, numComponentsRequested);

	ASSERT_OR_DIE(imageData != nullptr, "ERROR: Could not find texture");

	UINT miscFlags;
	UINT bindFlagsDesc;
	if (generateMips) {
		miscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		bindFlagsDesc = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else {
		miscFlags = 0;
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = static_cast<UINT>(m_textureSize.x);
	desc.Height = static_cast<UINT>(m_textureSize.y);
	desc.MipLevels = static_cast<UINT>(1);
	desc.ArraySize = static_cast<UINT>(1);
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT; //Default for now, may optimize later
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));

	data.pSysMem = imageData;
	data.SysMemPitch = m_textureSize.x * numComponents; //WARNING: Assuming RGBA8

	HRESULT hr = GetDevice()->CreateTexture2D(&desc, &data, &m_textureHandle);

	ASSERT_OR_DIE(SUCCEEDED(hr), "ERROR: Could not create D3D11 texture.");

	if (generateMips) {
		UseAsShaderResourceView();
		//GetDeviceContext()->GenerateMips(GetSRV());
	}

	size_t hash = std::hash<String>()(String(imageFilePath));
	s_textureRegistry.insert(D3D11TextureMapPair(hash, this));
}


//---------------------------------------------------------------------------------------------------------------------------
Texture2D::~Texture2D() {

	for (size_t i = 0; i < m_resourceViews.size(); i++) {
		delete m_resourceViews[i];
	}

	//if(m_textureHandle->)
	//m_textureHandle->Release();
}


//---------------------------------------------------------------------------------------------------------------------------
void Texture2D::UseAsDepthStencilView() {

	m_resourceViews[RESOURCE_TYPE_DEPTH_STENCIL_VIEW] = new D3D11Resource(m_textureHandle, RESOURCE_TYPE_DEPTH_STENCIL_VIEW);
}


//---------------------------------------------------------------------------------------------------------------------------
void Texture2D::UseAsRenderTargetView() {

	m_resourceViews[RESOURCE_TYPE_RENDER_TARGET_VIEW] = new D3D11Resource(m_textureHandle, RESOURCE_TYPE_RENDER_TARGET_VIEW);
}


//---------------------------------------------------------------------------------------------------------------------------
void Texture2D::UseAsShaderResourceView() {

	m_resourceViews[RESOURCE_TYPE_SHADER_RESOURCE_VIEW] = new D3D11Resource(m_textureHandle, RESOURCE_TYPE_SHADER_RESOURCE_VIEW);
}


//---------------------------------------------------------------------------------------------------------------------------
void Texture2D::UseAsUnorderedAccessView() {

	m_resourceViews[RESOURCE_TYPE_UNORDERED_ACCESS_VIEW] = new D3D11Resource(m_textureHandle, RESOURCE_TYPE_UNORDERED_ACCESS_VIEW);
}