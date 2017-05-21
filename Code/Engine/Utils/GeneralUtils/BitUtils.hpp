#pragma once

#include <bitset>
#include "Engine/General/Core/EngineCommon.hpp"

namespace BitUtils {
	inline void SetBit(byte& bitFlags, byte bitMask) {
		bitFlags |= bitMask;
		bitFlags = bitFlags | bitMask;
	}
	inline void SetBit(int& bitFlags, int bitMask) {
		bitFlags |= bitMask;
		bitFlags = bitFlags | bitMask;
	}

	inline bool IsBitSet(byte bitFlags, byte bitMask) {
		return (bitFlags & bitMask) != 0;
	}
	inline bool IsBitSet(int bitFlags, int bitMask) {
		return (bitFlags & bitMask) != 0;
	}

	inline void ClearBit(byte& bitFlags, byte bitMask) {
		bitFlags &= ~bitMask;
	}
	inline void ClearBit(int& bitFlags, int bitMask) {
		bitFlags &= ~bitMask;
	}

	String GetBitsAsString(byte bitsToOutput);
	String GetBitsAsString(int bitsToOutput);
}