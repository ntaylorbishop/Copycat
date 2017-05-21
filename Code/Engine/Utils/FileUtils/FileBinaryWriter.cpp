#include "Engine/Utils/FileUtils/FileBinaryWriter.hpp"

FileBinaryWriter::FileBinaryWriter() 
	: m_fileHandle(nullptr)
{ }

bool FileBinaryWriter::Open(const String& filename, bool append) {

	String mode;

	if (append) {
		mode = "ab";
	}
	else {
		mode = "wb";
	}

	errno_t error = fopen_s(&m_fileHandle, filename.c_str(), mode.c_str()); 
	
	if (error != 0) {
		DebuggerPrintf("file open error\n");
		return false;
	}
	
	return true;
}
void FileBinaryWriter::Close() {
	fclose(m_fileHandle);
}

VIRTUAL size_t FileBinaryWriter::WriteBytes(void* src, size_t numBytes) {
	return fwrite(src, sizeof(byte), numBytes, m_fileHandle);
}