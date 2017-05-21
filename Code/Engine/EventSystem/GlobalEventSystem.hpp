#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"

typedef void(EventCallback)(NamedProperties& properties);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OBJECT REGISTRATIONS STRUCTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
struct RegisteredObjectBase {
	virtual ~RegisteredObjectBase() { }
	virtual void Execute(NamedProperties& np) = 0;
	virtual void* GetObjectAddress() const { return nullptr; }
};


//---------------------------------------------------------------------------------------------------------------------------
template <typename T_ObjectType, typename T_ObjectMethod>
struct RegisteredObjectMethod : public RegisteredObjectBase {

	RegisteredObjectMethod(T_ObjectType* obj, T_ObjectMethod objMethod) : m_object(obj), m_method(objMethod) { }

	//typedef void (T_ObjectType::*ObjectMethodPtrType)(NamedProperties& params)

	T_ObjectType* m_object;
	T_ObjectMethod m_method;

	void Execute(NamedProperties& params) {
		(m_object->*m_method)(params);
	}

	VIRTUAL void* GetObjectAddress() const override { return (void*)m_object; }
};


//---------------------------------------------------------------------------------------------------------------------------
struct RegisteredFunction : public RegisteredObjectBase {

	RegisteredFunction(EventCallback* globalCallback) : m_callback(globalCallback) { }

	EventCallback* m_callback;
	void Execute(NamedProperties& params) {
		m_callback(params);
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EVENT SYSTEM MAIN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class EventSystem {
public:
	//INIT
	static void Initialize();
	static void Shutdown();

	//Register
	static void RegisterEventCallback(const String& eventName, EventCallback* globalCallbackFunc);
	
	template <typename T_Object, typename T_ObjectMethod>
	static void RegisterEventCallback(const String& eventName, T_Object* obj, T_ObjectMethod methodCallback);

	template <typename T>
	static void UnregisterObject(T* objWithMethodsRegistered);

	//Fire
	static void FireEvent(const String& eventName);
	static void FireEvent(const String& eventName, NamedProperties& eventProperties);

private:
	std::map<String, std::vector<RegisteredObjectBase*>> m_subscribers;

	static EventSystem* s_theEventSystem;
};

typedef std::map<String, std::vector<RegisteredObjectBase*>> SubscriberMap;
typedef std::map<String, std::vector<RegisteredObjectBase*>>::iterator SubscriberMapIterator;
typedef std::pair<String, std::vector<RegisteredObjectBase*>> SubscriberMapPair;
typedef std::vector<RegisteredObjectBase*> SubscriberList;
typedef std::vector<RegisteredObjectBase*>::iterator SubscriberListIterator;


////////////////////////////////////////////////////
//------------------------------------------
//INLINES

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//REGISTER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T_Object, typename T_ObjectMethod>
STATIC void EventSystem::RegisterEventCallback(const String& eventName, T_Object* obj, T_ObjectMethod methodCallback) {

	RegisteredObjectBase* funcObj = new RegisteredObjectMethod<T_Object, T_ObjectMethod>(obj, methodCallback);

	SubscriberMapIterator mapIt = s_theEventSystem->m_subscribers.find(eventName);

	if (mapIt != s_theEventSystem->m_subscribers.end()) {

		mapIt->second.push_back(funcObj);
	}
	else {
		std::vector<RegisteredObjectBase*> newSubscriberList;
		newSubscriberList.push_back(funcObj);
		s_theEventSystem->m_subscribers.insert(SubscriberMapPair(eventName, newSubscriberList));
	}
}



//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
STATIC void EventSystem::UnregisterObject(T* objWithMethodsRegistered) {

	SubscriberMapIterator mapIt = s_theEventSystem->m_subscribers.begin();

	for (mapIt; mapIt != s_theEventSystem->m_subscribers.end(); ++mapIt) {

		SubscriberList& eventSubscriberList = mapIt->second;
		SubscriberListIterator listIt = eventSubscriberList.begin();

		for (listIt; listIt != eventSubscriberList.end();) {

			if (listIt->GetObjectAddress() == (void*)objWithMethodsRegistered) {
				listIt = eventSubscriberList.erase(listIt);
			}
			else {
				++listIt;
			}
		}
	}

	/*
	virtual void* GetObjectAddress()
	return (void*)m_object;
	*/
}

