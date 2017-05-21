#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include <string>
#include <vector>
#include <stdarg.h>
#include <algorithm>

namespace StringUtils {
	const std::string Stringf(const char* format, ...);
	const std::string Stringf(const int maxLength, const char* format, ...);

	void TokenizeBuffer(const std::vector<unsigned char>& buffer, char delimiter, std::vector<String>& outTokens);
	void TokenizeBuffer(const String& buffer, char delimiter, std::vector<String>& outTokens);

	void ToLowerCase(String& outStrToParse);

	String GetRidOfAllOneCharacter(const String& strToParse, char charToRid);
	String ReverseString(const String& strToParse);

	String ToString(char c);
	String FormatBytesForOutput(int numBytes);
	inline String ToString(bool b) { return (b == false) ? ("0") : ("1"); }

	char* FindLast(const char* str, const char* strToFind);

	void CStringCopy(char* dest, const char* src, uint bufferSize);

	void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

	void Trim(String* str, char delimiter);
}