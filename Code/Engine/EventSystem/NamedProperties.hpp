#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

#include <map>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum ePropertyGetResult {
	PGR_SUCCESS = 0,
	PGR_WRONG_TYPE,
	PGR_NOT_PRESENT,
	PGR_EMPTY
};

enum ePropertySetResult {
	PSR_SET_EXISTING = 0,
	PSR_ADDED_NEW_PROPERTY,
	PSR_EXISTS_BUT_WRONG_TYPE
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BASE AND TYPED BASE PROPERTY STRUCTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct NamedBaseProperty {
	virtual ~NamedBaseProperty() { }
};

template <typename T>
struct TypedNamedProperty : public NamedBaseProperty {
public:
	TypedNamedProperty(const T& data)
		: m_data(data)
	{ }

	T m_data;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NAMED PROPERTIES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class NamedProperties {
public:
	//STRUCTORS
	NamedProperties() { }
	template <typename T>
	NamedProperties(const String& property1, const T& data1);
	template <typename T, typename S>
	NamedProperties(const String& property1, const T& data1, const String& property2, const S& data2);
	template <typename T, typename S, typename U>
	NamedProperties(const String& property1, const T& data1, const String& property2, const S& data2, const String& property3, const U& data3);

	inline ~NamedProperties();

	//GET SET
	template <typename T>
	ePropertyGetResult GetProperty(const String& name, T& out);

	template <typename T>
	ePropertySetResult SetProperty(const String& name, const T& dataToSet);

private:
	std::map<String, NamedBaseProperty*> m_properties;
};

////////////////////////////////////////////////////
//------------------------------------------
//INLINES AND TEMPLATE DEFS

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
NamedProperties::NamedProperties(const String& property1, const T& data1) {

	NamedBaseProperty* newProperty1 = new TypedNamedProperty<T>(data1);
	m_properties.insert(std::pair<String, NamedBaseProperty*>(property1, newProperty1));
}



//---------------------------------------------------------------------------------------------------------------------------
template <typename T, typename S>
NamedProperties::NamedProperties(const String& property1, const T& data1, const String& property2, const S& data2) {

	NamedBaseProperty* newProperty1 = new TypedNamedProperty<T>(data1);
	m_properties.insert(std::pair<String, NamedBaseProperty*>(property1, newProperty1));
	NamedBaseProperty* newProperty2 = new TypedNamedProperty<S>(data2);
	m_properties.insert(std::pair<String, NamedBaseProperty*>(property2, newProperty2));
}



//---------------------------------------------------------------------------------------------------------------------------
template <typename T, typename S, typename U>
NamedProperties::NamedProperties(const String& property1, const T& data1, const String& property2, const S& data2, const String& property3, const U& data3) {

	NamedBaseProperty* newProperty1 = new TypedNamedProperty<T>(data1);
	m_properties.insert(std::pair<String, NamedBaseProperty*>(property1, newProperty1));
	NamedBaseProperty* newProperty2 = new TypedNamedProperty<S>(data2);
	m_properties.insert(std::pair<String, NamedBaseProperty*>(property2, newProperty2));
	NamedBaseProperty* newProperty3 = new TypedNamedProperty<U>(data3);
	m_properties.insert(std::pair<String, NamedBaseProperty*>(property3, newProperty3));
}

//---------------------------------------------------------------------------------------------------------------------------
NamedProperties::~NamedProperties() {
	
	std::map<String, NamedBaseProperty*>::iterator propertyIt = m_properties.begin();
	for (propertyIt; propertyIt != m_properties.end(); ++propertyIt) {
		delete propertyIt->second;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GET SET
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
ePropertyGetResult NamedProperties::GetProperty(const String& name, T& out) {
	//First do find based on string
	//If not found return err not found
	//If found, we return a NamedPropertyBase

	if (m_properties.empty()) {
		return PGR_EMPTY;
	}

	std::map<String, NamedBaseProperty*>::iterator propertyIt = m_properties.find(name);

	if (propertyIt != m_properties.end()) {

		NamedBaseProperty* baseProperty = propertyIt->second;
		TypedNamedProperty<T>* typedProperty = dynamic_cast<TypedNamedProperty<T>*>(baseProperty);

		if (!typedProperty) {
			return PGR_WRONG_TYPE;
		}
		else {
			out = typedProperty->m_data;
			return PGR_SUCCESS;
		}
	}
	else {
		return PGR_NOT_PRESENT;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
ePropertySetResult NamedProperties::SetProperty(const String& name, const T& dataToSet) {

	std::map<String, NamedBaseProperty*>::iterator propertyIt = m_properties.find(name);

	if (propertyIt != m_properties.end()) {

		NamedBaseProperty* baseProperty = propertyIt->second;
		TypedNamedProperty<T>* typedProperty = dynamic_cast<TypedNamedProperty<T>*>(baseProperty);

		if (!typedProperty) {
			return PSR_EXISTS_BUT_WRONG_TYPE;
		}
		else {
			typedProperty->m_data = dataToSet;
			return PSR_SET_EXISTING;
		}
	}
	else {
		TypedNamedProperty<T>* newProperty = new TypedNamedProperty<T>(dataToSet);
		m_properties.insert(std::pair<String, NamedBaseProperty*>(name, newProperty));
		return PSR_ADDED_NEW_PROPERTY;
	}
}