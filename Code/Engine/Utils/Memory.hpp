#pragma once

#include "Engine/General/Core/EngineCommon.hpp"


namespace Memory {

	void Copy(byte* bufferStart, size_t idxToWriteTo, size_t bufferSize, byte* blockToCopy, size_t sizeOfBlock);

}