#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class SSBO {
public:
	//STRUCTORS
	SSBO() : m_id(-1), m_buffer(nullptr), m_curr(nullptr), m_bufferSize(0) { }
	~SSBO() { }

	//INIT DESTROY
	void Generate(size_t bufferSize);
	void Destroy();

	//BIND AND PUSH
	void Bind(uint layoutBindPoint);
	void BindDataToGPU();
	void UpdateDataOnGPU();
	void CopyDataFromGPU();

	//READ WRITE
	template <typename T>
	void Write(const T& data);

	template <typename T>
	T Read();

	void DeleteLocalCopy()				{ free(m_buffer);						}
	void ClearToZero()					{ memset(m_buffer, 0, m_bufferSize);	}	
	void ResetHead()					{ m_curr = m_buffer;					}

	//GETTERS SETTERS
	byte* GetBuffer()					{ return m_buffer;						}
	size_t GetBufferSize()				{ return m_bufferSize;					}
	void SetBufferHead(size_t pos)		{ m_curr = m_buffer; m_curr += pos;		}
	void ZeroOutBufferAndResetHead();

	void WriteBytes(byte* data, int size);
private:


	byte* m_buffer;
	byte* m_curr;
	size_t m_bufferSize;

	int m_id;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEMPLATES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void SSBO::Write(const T& data) {

	T copy = data;
	WriteBytes((byte*)&copy, sizeof(T));
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T SSBO::Read() {
	T* ptrToObj = (T*)m_curr;
	m_curr += sizeof(T);
	return *ptrToObj;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline void SSBO::ZeroOutBufferAndResetHead() {

	ResetHead();
	memset(m_buffer, 0, m_bufferSize);
}