#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "ThirdParty/Parsers/xmlParser.h"
#include "Engine/Utils/StringUtils.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/RangeInt.hpp"
#include "Engine/Math/Range.hpp"
#include "Engine/Math/RangeFloat.hpp"

namespace XMLUtils {

	//---------------------------------------------------------------------------------------------------------------------------
	//STRUCTS
	//---------------------------------------------------------------------------------------------------------------------------
	struct Attribute {
		Attribute(const String& name, const String& value) : name(name), value(value) { }

		String name;
		String value;
	};
	
	IntVector2		ParseIntVector2(const String& attribValue);
	Vector2			ParseVector2(const String& attribValue);
	Vector3			ParseVector3(const String& attribValue);
	Vector4			ParseVector4(const String& attribValue);
	int				ParseInt(const String& attribValue);
	uint			ParseUint(const String& attribValue);
	char			ParseChar(const String& attribValue);
	RGBA			ParseHexColor(const String& attribValue);
	RGBA			ParseFloatColor(const String& attribValue);
	float			ParseFloat(const String& attribValue);
	AABB2			ParseAABB2(const String& attribValue);
	Range<int>		ParseRangeInt(const String& attribValue, char delimiter);
	Range<float>	ParseRangeFloat(const String& attribValue);
	Range<int>		ParseRangeInt(const String& attribValue);
	Range<Vector3>	ParseRangeVector3(const String& attribValue);
	bool			ParseBool(const String& attribValue);

	bool GetAttributeByName(const String& name, const XMLNode& node, Attribute& outAttrib);
	bool DoesAttributeExist(const String& attribName, const std::vector<Attribute>& attribPairs);
	bool GetAllAttributesOfNode(const XMLNode& node, std::vector<Attribute>& outAttribPairs);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline int XMLUtils::ParseInt(const String& attribValue) {
	return stoi(attribValue);
}


//---------------------------------------------------------------------------------------------------------------------------
inline uint XMLUtils::ParseUint(const String& attribValue) {
	return (uint)ParseInt(attribValue);
}


//---------------------------------------------------------------------------------------------------------------------------
inline char XMLUtils::ParseChar(const String& attribValue) {
	return attribValue[0];
}