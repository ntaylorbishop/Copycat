#include "Engine/Utils/FileUtils/FileBinaryReader.hpp"

FileBinaryReader::FileBinaryReader()
	: m_fileHandle(nullptr)
	, fileIdx(0)
{ }

bool FileBinaryReader::Open(const String& filename, bool append) {
	UNREFERENCED_PARAMETER(append);
	errno_t error = fopen_s(&m_fileHandle, filename.c_str(), "rb");

	if (error != 0) {
		return false;
	}

	return true;
}
VIRTUAL size_t FileBinaryReader::ReadBytes(void* out, size_t numBytes) {
	//fseek();
	return fread(out, sizeof(byte), numBytes, m_fileHandle);
}
void FileBinaryReader::Close() {
	fclose(m_fileHandle);
}