#include "Engine/Utils/Memory.hpp"


//---------------------------------------------------------------------------------------------------------------------------
void Memory::Copy(byte* bufferStart, size_t idxToWriteTo, size_t bufferSize, byte* blockToCopy, size_t sizeOfBlock) {

	#ifdef _DEBUG
	ASSERT_OR_DIE((bufferSize - idxToWriteTo) >= sizeOfBlock, "ERROR: Buffer overflow.");
	memcpy_s(bufferStart + idxToWriteTo, bufferSize, blockToCopy, sizeOfBlock);
	#else
	memcpy_s(bufferStart + idxToWriteTo, bufferSize, blockToCopy, sizeOfBlock);
	#endif
}
