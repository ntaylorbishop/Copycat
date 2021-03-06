#include "Engine/Renderer/D3D11/Mesh/D3D11Mesh.hpp"

STATIC D3D11Mesh* D3D11Mesh::s_meshList = nullptr;


//---------------------------------------------------------------------------------------------------------------------------
D3D11Mesh::D3D11Mesh(eVertexType vertType, size_t numVerts)
	: m_vertType(vertType)
	, m_numVerts(0)
	, m_vertByteSize(GetVertexByteSize(vertType))
	, m_bufferSize(m_vertByteSize * numVerts)
{
	m_pVertData = (byte*)malloc(m_vertByteSize * numVerts);
	//AddMeshToList();
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::Destroy() {

	delete m_pVertData;
	m_pVertData = nullptr;
	delete m_pIndData;
	m_pIndData = nullptr;
	m_pVertBuffer->Release();
	m_pIndBuffer->Release();
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::AddMeshToList() {

	if (s_meshList == nullptr) {
		EventSystem::RegisterEventCallback("Shutdown", &D3D11Mesh::BindListShutdownEvent);
		s_meshList = this;
		return;
	}

	D3D11Mesh* curr = s_meshList;

	while (curr->m_next) {
		curr = curr->m_next;
	}

	curr->m_next = this;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void D3D11Mesh::BindListShutdownEvent(NamedProperties& np) {

	//WARNING: Need to figure out how to delete without 
	return;

	UNUSED(np);

	D3D11Mesh* curr = s_meshList;
	while (curr) {

		D3D11Mesh* next = curr->m_next;
		curr->Destroy();
		delete curr;
		curr = next;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::InitializeMeshOnDevice(uint32_t* indBuffer, uint numInds) {

	CreateVertexBufferOnDevice();
	BindVertBufferToDeviceWindow();

	// Create index buffer
	SetIndexBuffer(indBuffer, numInds * sizeof(uint32_t), numInds);
	CreateIndexBufferOnDevice();
	BindIndBufferToDeviceWindow();
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::AddVertex(const Vector3& pos, const RGBA& col) {
	CreateVertex(m_pVertData + m_numVerts * m_vertByteSize, m_bufferSize, pos, col);
	m_numVerts++;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::AddVertex(const Vector3& pos, const RGBA& col, const Vector2& tex) {
	CreateVertex(m_pVertData + m_numVerts * m_vertByteSize, m_bufferSize, pos, col, tex);
	m_numVerts++;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::AddVertex(const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& normal) {
	CreateVertex(m_pVertData + m_numVerts * m_vertByteSize, m_bufferSize, pos, col, tex, normal);
	m_numVerts++;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::AddVertex(const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& tangent, const Vector4& bitangent, const Vector4& normal) {
	CreateVertex(m_pVertData + m_numVerts * m_vertByteSize, m_bufferSize, pos, col, tex, tangent, bitangent, normal);
	m_numVerts++;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::AddVertex(const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& tangent, const Vector4& bitangent, const Vector4& normal, const Vector4& boneWeights, const IntVector4& boneInds) {
	CreateVertex(m_pVertData + m_numVerts * m_vertByteSize, m_bufferSize, pos, col, tex, tangent, bitangent, normal, boneWeights, boneInds);
	m_numVerts++;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::CreateVertexBufferOnDevice() {

	ZeroMemory(&m_bufferDesc, sizeof(m_bufferDesc));
	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = m_vertByteSize * m_numVerts;
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&m_initData, sizeof(m_initData));
	m_initData.pSysMem = m_pVertData;
	HRESULT hr = RHIDeviceWindow::Get()->m_pd3dDevice->CreateBuffer(&m_bufferDesc, &m_initData, &m_pVertBuffer);

	if (FAILED(hr))
		ERROR_AND_DIE("HR FAILED");
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::CreateIndexBufferOnDevice() {

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(uint32_t) * m_numInds;
	m_bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_initData.pSysMem = m_pIndData;
	HRESULT hr = RHIDeviceWindow::Get()->m_pd3dDevice->CreateBuffer(&m_bufferDesc, &m_initData, &m_pIndBuffer);
	if (FAILED(hr))
		ERROR_AND_DIE("HR FAILED");

}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::BindVertBufferToDeviceWindow() {

	UINT stride = m_vertByteSize;
	UINT offset = 0;
	RHIDeviceWindow::Get()->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertBuffer, &stride, &offset);
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::BindIndBufferToDeviceWindow() {

	RHIDeviceWindow::Get()->m_pDeviceContext->IASetIndexBuffer(m_pIndBuffer, DXGI_FORMAT_R32_UINT, 0);
	RHIDeviceWindow::Get()->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Mesh::SetIndexBuffer(uint32_t* indBuffer, size_t bufferSize, uint numInds) {

	m_pIndData = new uint32_t[numInds];
	memcpy_s(m_pIndData, sizeof(uint32_t) * numInds, indBuffer, bufferSize);
	m_numInds = numInds;
}