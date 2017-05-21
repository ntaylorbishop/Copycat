#include "Engine/Utils/GeneralUtils/Endianness.hpp"

eEndianness SystemEndianness() {

	union {
		byte bData[4];
		uint32_t uiData;
	} data;

	data.uiData = 0x04030201;
	return (data.bData[0] == 0x01) ? LITTLE_ENDIAN : BIG_ENDIAN;
}