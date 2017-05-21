#include "Engine/Renderer/Structures/SSBO.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void SSBO::Generate(size_t bufferSize) {
	GL_CHECK(glGenBuffers(1, (GLuint*)&m_id));
	m_bufferSize = bufferSize;
	m_buffer = (byte*)malloc(m_bufferSize);
	memset(m_buffer, 0, m_bufferSize);
	m_curr = m_buffer;
}


//---------------------------------------------------------------------------------------------------------------------------
void SSBO::Destroy() {
	if (m_buffer) {
		GL_CHECK(glDeleteBuffers(1, (GLuint*)&m_id));
		free(m_buffer);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BIND AND PUSH
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void SSBO::Bind(uint layoutBindPoint) {

	GL_CHECK(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, layoutBindPoint, m_id));
}


//---------------------------------------------------------------------------------------------------------------------------
void SSBO::BindDataToGPU() {

	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)m_id));
	GL_CHECK(glBufferData(GL_SHADER_STORAGE_BUFFER, m_bufferSize, m_buffer, GL_DYNAMIC_COPY));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}


//---------------------------------------------------------------------------------------------------------------------------
void SSBO::UpdateDataOnGPU() {
	
	//TODO: Figure out what to do with this


	//GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
	//
	////GL_CHECK(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_bufferSize, m_buffer));
	//GLvoid* ptrToGPUBuffer = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	//
	//ASSERT_OR_DIE(ptrToGPUBuffer != nullptr, "ERROR: Could not grab pointer to gpu buffer for SSBO.");
	//
	//memcpy(ptrToGPUBuffer, m_buffer, m_bufferSize);
	//GL_CHECK(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
}


//---------------------------------------------------------------------------------------------------------------------------
void SSBO::CopyDataFromGPU() {

	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));

	GLvoid* ptrToGPUBuffer = nullptr;
	GL_CHECK(ptrToGPUBuffer = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));

	ASSERT_OR_DIE(ptrToGPUBuffer != nullptr, "ERROR: Could not grab pointer to gpu buffer for SSBO.");

	memcpy(m_buffer, ptrToGPUBuffer, m_bufferSize);
	GL_CHECK(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE READ WRITE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void SSBO::WriteBytes(byte* data, int size) {

	ASSERT_OR_DIE(size > 0, "ERROR: Must write more than 0 bytes.");
	ASSERT_OR_DIE((uint)(m_curr + size - m_buffer) <= m_bufferSize, "ERROR: Out of bounds on SSBO writing. Buffer too small.");

	memcpy(m_curr, data, size);
	m_curr += size;
}