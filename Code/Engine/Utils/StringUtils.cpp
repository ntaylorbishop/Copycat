#include "Engine/Utils/StringUtils.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRINGF FUNCS
//---------------------------------------------------------------------------------------------------------------------------
const int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;

const std::string StringUtils::Stringf( const char* format, ... ) {
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}

const std::string StringUtils::Stringf( const int maxLength, const char* format, ... ) {
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new(MEMORY_GENERAL) char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}

//---------------------------------------------------------------------------------------------------------------------------
//OTHER
//---------------------------------------------------------------------------------------------------------------------------
void StringUtils::TokenizeBuffer(const std::vector<unsigned char>& buffer, char delimiter, std::vector<String>& outTokens) {
	
	std::string str = "";
	for (unsigned int i = 0; i < buffer.size(); i++) {
		char currChar = (unsigned char)buffer[i];

		if (currChar != delimiter) {
			str.push_back(currChar);
		}
		else {
			if (str != "") {
				outTokens.push_back(str);
				str = "";
			}
			else {
				continue;
			}
		}
	}
}
void StringUtils::TokenizeBuffer(const String& buffer, char delimiter, std::vector<String>& outTokens) {

	std::string str = "";
	for (unsigned int i = 0; i < buffer.size(); i++) {
		char currChar = buffer[i];

		if (currChar != delimiter) {
			str.push_back(currChar);
		}
		else {
			if (str != "") {
				outTokens.push_back(str);
				str = "";
			}
			else {
				continue;
			}
		}
	}

	if (str != "") {
		outTokens.push_back(str);
	}
}

void StringUtils::ToLowerCase(String& outStrToParse) {
	std::transform(outStrToParse.begin(), outStrToParse.end(), outStrToParse.begin(), ::tolower);
}

String StringUtils::GetRidOfAllOneCharacter(const String& strToParse, char charToRid) {
	String str;

	for (size_t i = 0; i < strToParse.size(); i++) {
		if (strToParse[i] != charToRid) {
			str.push_back(strToParse[i]);
		}
	}

	return str;
}

String StringUtils::ReverseString(const String& strToParse) {
	String str;

	for (size_t i = strToParse.size() - 1; i > 0; i--) {
		str.push_back(strToParse[i]);
	}

	return str;
}
String StringUtils::ToString(char c) {
	String str = "";
	str.push_back(c);
	return str;
}

char* StringUtils::FindLast(const char* str, const char* strToFind) {
	str;
	strToFind;
	return "";
}
void StringUtils::CStringCopy(char* dest, const char* src, uint bufferSize) {
	
	for (uint i = 0; i < bufferSize - 2; i++) {
		dest[i] = src[i];
	}

	dest[bufferSize - 1] = '\0';
}

String StringUtils::FormatBytesForOutput(int numBytes) {
	float bytes = (float)numBytes;
	String suffix = "B";

	if (abs(bytes) >= 1000.f) {
		bytes /= 1024.f;
		suffix = "KB";

		if (abs(bytes) >= 1000.f) {
			bytes /= 1024.f;
			suffix = "MB";

			if (abs(bytes) >= 1000.f) {
				bytes /= 1024.f;
				suffix = "GB";
			}
		}
	}
	return Stringf("%.1f%s", bytes, suffix.c_str());
}

void StringUtils::ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

void StringUtils::Trim(String* str, char delimiter) {

	if (str->at(0) == delimiter) {
		str->erase(str->begin() + 0);
	}

	if (str->at(str->length() - 1) == delimiter) {
		str->erase(str->begin() + str->length());
	}
}