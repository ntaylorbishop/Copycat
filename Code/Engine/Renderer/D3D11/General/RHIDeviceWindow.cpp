#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Input/InputSystem.hpp"

STATIC RHIDeviceWindow* RHIDeviceWindow::s_deviceWindow = nullptr;


UINT SCREENSIZE_X = 3840;
UINT SCREENSIZE_Y = 2160;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INIT DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void RHIDeviceWindow::Initialize(HINSTANCE applicationInstanceHandle, int nCmdShow) {

	s_deviceWindow = new RHIDeviceWindow(applicationInstanceHandle, nCmdShow);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void RHIDeviceWindow::Destroy() {

	delete s_deviceWindow;
	s_deviceWindow = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
RHIDeviceWindow::RHIDeviceWindow(HINSTANCE applicationInstanceHandle, int nCmdShow) {

	InitWindow(applicationInstanceHandle, nCmdShow);
	CreateDevice();
}


//---------------------------------------------------------------------------------------------------------------------------
RHIDeviceWindow::~RHIDeviceWindow() {

	CleanupDeviceAndWindow();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void RHIDeviceWindow::ClearAndPresent() {

	float col[] = { 1.f, 0.f, 0.f, 1.f };
	s_deviceWindow->m_pDeviceContext->ClearRenderTargetView(s_deviceWindow->m_pRenderTargetView, col);
	s_deviceWindow->m_pSwapChain->Present(0, 0);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void RHIDeviceWindow::InitWindow(HINSTANCE applicationInstanceHandle, int nCmdShow) {

	SetProcessDPIAware();

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowsMessageHandlingProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = applicationInstanceHandle;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"D3D Renderer";
	wcex.hIconSm = NULL;
	RegisterClassEx(&wcex);

	// Create window
	m_hInst = applicationInstanceHandle;
	RECT rc = { 0, 0, SCREENSIZE_X, SCREENSIZE_Y };
	AdjustWindowRect(&rc, WS_VISIBLE, FALSE);
	//m_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 1: Direct3D 11 Basics",
	//	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	//	CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, applicationInstanceHandle,
	//	nullptr);
	m_hWnd = CreateWindow(L"D3D Renderer", L"D3D Renderer",
		WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, applicationInstanceHandle,
		nullptr);
	SetWindowLong(m_hWnd, GWL_STYLE, 0);

	ShowWindow(m_hWnd, nCmdShow);
}


//---------------------------------------------------------------------------------------------------------------------------
void RHIDeviceWindow::CreateDevice() {

	RECT rc = { 0, 0, SCREENSIZE_X, SCREENSIZE_Y };
	HRESULT hr = S_OK;

	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pDeviceContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pDeviceContext);
		}

		if (SUCCEEDED(hr))
			break;
	}

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			(void)m_pDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_pd3dDevice, m_hWnd, &sd, nullptr, nullptr, &m_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();


	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
}


//---------------------------------------------------------------------------------------------------------------------------
void RHIDeviceWindow::CleanupDeviceAndWindow() {

	if (m_pDeviceContext) {
		m_pDeviceContext->ClearState();
	}
	if (m_pVertexBuffer) {
		m_pVertexBuffer->Release();
	}
	if (m_pVertexLayout) {
		m_pVertexLayout->Release();
	}
	if (m_pVertexShader) {
		m_pVertexShader->Release();
	}
	if (m_pPixelShader) {
		m_pPixelShader->Release();
	}
	if (m_pRenderTargetView) {
		m_pRenderTargetView->Release();
	}
	if (m_pSwapChain1) {
		m_pSwapChain1->Release();
	}
	if (m_pSwapChain) {
		m_pSwapChain->Release();
	}
	if (m_pImmediateContext1) {
		m_pImmediateContext1->Release();
	}
	if (m_pDeviceContext) {
		m_pDeviceContext->Release();
	}
	if (m_pd3dDevice1) {
		m_pd3dDevice1->Release();
	}
	if (m_pd3dDevice) {
		m_pd3dDevice->Release();
	}
}