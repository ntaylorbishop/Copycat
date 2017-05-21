#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRING PARSING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
IntVector2 XMLUtils::ParseIntVector2(const String& attribValue) {
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, ',', tokens);

	return IntVector2(stoi(tokens[0]), stoi(tokens[1]));
}


//---------------------------------------------------------------------------------------------------------------------------
Vector2 XMLUtils::ParseVector2(const String& attribValue) {

	String strToParse = StringUtils::GetRidOfAllOneCharacter(attribValue, ' ');
	strToParse = StringUtils::GetRidOfAllOneCharacter(strToParse, '(');
	strToParse = StringUtils::GetRidOfAllOneCharacter(strToParse, ')');

	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(strToParse, ',', tokens);

	return Vector2(stof(tokens[0]), stof(tokens[1]));
}


//---------------------------------------------------------------------------------------------------------------------------
Vector3	XMLUtils::ParseVector3(const String& attribValue) {

	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, ',', tokens);

	ASSERT_OR_DIE(tokens.size() == 3, "ERROR: XML vec3 value must have three parameters.");
	
	Vector3 outVec;

	//X
	String xTok = tokens[0];
	StringUtils::Trim(&xTok, '(');
	StringUtils::Trim(&xTok, ')');
	outVec.x = stof(xTok);

	//Y
	String yTok = tokens[1];
	StringUtils::Trim(&yTok, '(');
	StringUtils::Trim(&yTok, ')');
	outVec.y = stof(yTok);

	//Z
	String zTok = tokens[2];
	StringUtils::Trim(&zTok, '(');
	StringUtils::Trim(&zTok, ')');
	outVec.z = stof(zTok);

	return outVec;
}


//---------------------------------------------------------------------------------------------------------------------------
Vector4	XMLUtils::ParseVector4(const String& attribValue) {

	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, ',', tokens);

	ASSERT_OR_DIE(tokens.size() == 4, "ERROR: Vector4 must have three values");

	return Vector4(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
}


//---------------------------------------------------------------------------------------------------------------------------
RGBA XMLUtils::ParseHexColor(const String& attribValue) {

	String strToParse = StringUtils::GetRidOfAllOneCharacter(attribValue, ' ');

	//Should look like: (0, 255, 128, 100)
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(strToParse, ',', tokens);

	return RGBA((float)stoi(tokens[0]) / 255.f, (float)stoi(tokens[1]) / 255.f, (float)stoi(tokens[2]) / 255.f, (float)stoi(tokens[3]) / 255.f);
}


//---------------------------------------------------------------------------------------------------------------------------
RGBA XMLUtils::ParseFloatColor(const String& attribValue) {
	String strToParse = StringUtils::GetRidOfAllOneCharacter(attribValue, ' ');

	//Should look like: (0, 255, 128, 100)
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(strToParse, ',', tokens);

	return RGBA(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
}


//---------------------------------------------------------------------------------------------------------------------------
Range<int> XMLUtils::ParseRangeInt(const String& attribValue, char delimiter) {
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, delimiter, tokens);

	return (tokens.size() == 1) ? (Range<int>(stoi(tokens[0]), stoi(tokens[0]))) : (Range<int>(stoi(tokens[0]), stoi(tokens[1])));
}


//---------------------------------------------------------------------------------------------------------------------------
float XMLUtils::ParseFloat(const String& attribValue) {
	float retVal = 0.f;

	try {
		retVal = std::stof(attribValue);
	}
	catch (int e) {
		UNREFERENCED_PARAMETER(e);
		ERROR_AND_DIE("ERROR: Could not parse float from XML attribute value.");
	}

	return retVal;
}


//---------------------------------------------------------------------------------------------------------------------------
AABB2 XMLUtils::ParseAABB2(const String& attribValue) {

	String str = attribValue;
	str = StringUtils::GetRidOfAllOneCharacter(str, ' ');
	str = StringUtils::GetRidOfAllOneCharacter(str, '(');
	str = StringUtils::GetRidOfAllOneCharacter(str, ')');
	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(str, ',', tokens);

	return (AABB2(Vector2(stof(tokens[0]), stof(tokens[1])), Vector2(stof(tokens[2]), stof(tokens[3]))));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ATTRIBUTES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
bool XMLUtils::GetAttributeByName(const String& name, const XMLNode& node, XMLUtils::Attribute& outAttrib) {

	for (int i = 0; i < node.nAttribute(); i++) {
		String attrib = node.getAttributeValue(i);
		if (name == attrib) {
			outAttrib = XMLUtils::Attribute(node.getAttributeName(i), node.getAttributeValue(i));
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
bool XMLUtils::DoesAttributeExist(const String& attribName, const std::vector<Attribute>& attribPairs) {
	bool exists = false;

	for (unsigned int i = 0; i < attribPairs.size(); i++) {
		String attribNameLower;
		StringUtils::ToLowerCase(attribNameLower);

		if (attribPairs[i].name == attribName) {
			exists = true;
		}
	}

	return exists;
}


//---------------------------------------------------------------------------------------------------------------------------
bool XMLUtils::GetAllAttributesOfNode(const XMLNode& node, std::vector<Attribute>& outAttribPairs) {

	if (!node.isEmpty()) {
		for (int i = 0; i < node.nAttribute(); i++) {
			outAttribPairs.push_back(Attribute(node.getAttributeName(i), node.getAttributeValue(i)));
		}
		return true;
	}
	else {
		DebuggerPrintf("XMLUtils: Cannot get attributes from NULL node.");
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Range<float> XMLUtils::ParseRangeFloat(const String& attribValue) {

	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, '~', tokens);

	return (tokens.size() == 1) ? (Range<float>(stof(tokens[0]), stof(tokens[0]))) : (Range<float>(stof(tokens[0]), stof(tokens[1])));
}


//---------------------------------------------------------------------------------------------------------------------------
Range<int> XMLUtils::ParseRangeInt(const String& attribValue) {

	std::vector<String> tokens;
	StringUtils::TokenizeBuffer(attribValue, '~', tokens);

	return (tokens.size() == 1) ? (Range<int>(stoi(tokens[0]), stoi(tokens[0]))) : (Range<int>(stoi(tokens[0]), stoi(tokens[1])));
}


//---------------------------------------------------------------------------------------------------------------------------
Range<Vector3> XMLUtils::ParseRangeVector3(const String& attribValue) {

	std::vector<String> vectorTokens;
	StringUtils::TokenizeBuffer(attribValue, '~', vectorTokens);

	if (vectorTokens.size() == 1) {
		Vector3 val = ParseVector3(vectorTokens[0]);
		return Range<Vector3>(val);
	}
	else if (vectorTokens.size() == 2) {
		Vector3 min = ParseVector3(vectorTokens[0]);
		Vector3 max = ParseVector3(vectorTokens[1]);
		return Range<Vector3>(min, max);
	}
	else {
		ERROR_AND_DIE("ERROR: Range Vector3 must have at least vector 3 defined.");
	}
}


//---------------------------------------------------------------------------------------------------------------------------
bool XMLUtils::ParseBool(const String& attribValue) {

	if (attribValue.length() == 0) {
		return (attribValue[0] == '0') ? (false) : (true);
	}

	if (attribValue == "false") {
		return false;
	}
	else if (attribValue == "true") {
		return true;
	}
	else {
		ERROR_AND_DIE("Not a valid boolean value defined in XML.");
	}
}