#include "Engine/Utils/GeneralUtils/BitUtils.hpp"

String BitUtils::GetBitsAsString(byte bitsToOutput) {
	std::bitset<8> output(bitsToOutput);
	return output.to_string<char, std::string::traits_type, std::string::allocator_type>();
}
String BitUtils::GetBitsAsString(int bitsToOutput) {
	std::bitset<32> output(bitsToOutput);
	return output.to_string<char, std::string::traits_type, std::string::allocator_type>();
}