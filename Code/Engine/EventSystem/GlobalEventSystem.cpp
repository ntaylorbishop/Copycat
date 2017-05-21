#include "Engine/EventSystem/GlobalEventSystem.hpp"

STATIC EventSystem* EventSystem::s_theEventSystem = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INIT DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void EventSystem::Initialize() {

	ASSERT_OR_DIE(s_theEventSystem == nullptr, "ERROR: Global Event System already initialized");
	s_theEventSystem = new EventSystem();
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void EventSystem::Shutdown() {

	ASSERT_OR_DIE(s_theEventSystem != nullptr, "ERROR: Global Event System either already destroyed or not initialized.");
	delete s_theEventSystem;
	s_theEventSystem = nullptr;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Register
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void EventSystem::RegisterEventCallback(const String& eventName, EventCallback* globalCallbackFunc) {

	RegisteredObjectBase* funcObj = new RegisteredFunction(globalCallbackFunc);

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fire
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void EventSystem::FireEvent(const String& eventName) {

	SubscriberMapIterator mapIt = s_theEventSystem->m_subscribers.find(eventName);

	if (mapIt != s_theEventSystem->m_subscribers.end()) {

		if (mapIt->second.empty()) { //Do a quick check to see if our subscriber list is empty and delete the node
			s_theEventSystem->m_subscribers.erase(mapIt);
			return;
		}
		else {
			for (uint i = 0; i < mapIt->second.size(); i++) {
				NamedProperties noData = NamedProperties();
				mapIt->second.at(i)->Execute(noData);
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void EventSystem::FireEvent(const String& eventName, NamedProperties& eventProperties) {

	SubscriberMapIterator mapIt = s_theEventSystem->m_subscribers.find(eventName);

	if (mapIt != s_theEventSystem->m_subscribers.end()) {

		if (mapIt->second.empty()) { //Do a quick check to see if our subscriber list is empty and delete the node
			s_theEventSystem->m_subscribers.erase(mapIt);
			return;
		}
		else {
			for (uint i = 0; i < mapIt->second.size(); i++) {
				mapIt->second.at(i)->Execute(eventProperties);
			}
		}
	}
}