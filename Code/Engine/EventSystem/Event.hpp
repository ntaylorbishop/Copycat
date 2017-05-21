#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

//Based off of code written by Anthony Cloudy
template <typename ...Args>
class Event
{
public:
	struct Subscription;
	//TYPEDEFS/////////////////////////////////////////////////////////////////////
	typedef void(SubscriptionCallback)(Subscription* sub, Args...);
	typedef void(FunctionCallback)(Args...);
	typedef void(EventCallback)(Subscription* sub, Args...);
	//STRUCTS/////////////////////////////////////////////////////////////////////
	struct Subscription
	{
		SubscriptionCallback* utilityCallback; // This is the C style function the trigger actually calls
		void* callback;         // Is the callback registered by the user
		void* argument;         // Additional data that goes with the subscription (optional)
	};
	//CONSTRUCTORS/////////////////////////////////////////////////////////////////////
	Event() {};
	//-----------------------------------------------------------------------------------
	~Event()
	{
		UnregisterAllSubscriptions();
	}
	//FUNCTIONS/////////////////////////////////////////////////////////////////////
	void RegisterFunction(FunctionCallback* cb)
	{
		RegisterSubscription(&EventFunctionCallback, cb, nullptr);
	}
	//-----------------------------------------------------------------------------------
	void UnregisterFunction(FunctionCallback* cb)
	{
		UnregisterSubscription(&EventFunctionCallback, cb, nullptr);
	}
	//-----------------------------------------------------------------------------------
	template <typename T>
	void RegisterMethod(T* object, void (T::*methodCallback)(Args...))
	{
		void* uniquePointerToCallback = *(void**)(&methodCallback);
		RegisterSubscription(&MethodCallback<T>, uniquePointerToCallback, object);
	}
	//-----------------------------------------------------------------------------------
	template <typename T>
	void UnregisterMethod(T* object, void (T::*methodCallback)(Args...))
	{
		void* uniquePointerToCallback = *(void**)(&methodCallback);
		UnregisterSubscription(&MethodCallback<T>, uniquePointerToCallback, object);
	}
	//-----------------------------------------------------------------------------------
	void UnregisterAllSubscriptions()
	{
		m_subscriptions.clear();
	}
	//-----------------------------------------------------------------------------------
	void Trigger(Args... args)
	{
		unsigned int size = m_subscriptions.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			auto& sub = m_subscriptions[i];
			sub.utilityCallback(&sub, args...);
			if (m_subscriptions.size() < size)
			{
				size = m_subscriptions.size();
				--i;
			}
		}
	}
	//STATIC FUNCTIONS/////////////////////////////////////////////////////////////////////
	template <typename T>
	static void MethodCallback(Subscription* sub, Args... args)
	{
		typedef void (T::* Callback)(Args...);
		Callback mcb = *(Callback*)(&sub->callback);
		T* objectPtr = (T*)sub->argument;
		(objectPtr->*mcb)(args...);
	}
	//-----------------------------------------------------------------------------------
	static void EventFunctionCallback(Subscription* sub, Args... args)
	{
		FunctionCallback* cb = (FunctionCallback*)(sub->callback);
		cb(args...);
	}
private:
	//PRIVATE FUNCTIONS/////////////////////////////////////////////////////////////////////
	void RegisterSubscription(void* utilityCallback, void* actualCallback, void* data)
	{
		Subscription sub;
		sub.utilityCallback = (SubscriptionCallback*)utilityCallback;
		sub.callback = actualCallback;
		sub.argument = data;
		m_subscriptions.push_back(sub);
	}
	//-----------------------------------------------------------------------------------
	void UnregisterSubscription(void* utilityCallback, void* actualCallback, void* data)
	{
		for (auto subIter = m_subscriptions.begin(); subIter != m_subscriptions.end(); ++subIter)
		{
			if (subIter->callback == actualCallback && subIter->utilityCallback == utilityCallback && subIter->argument == data)
			{
				m_subscriptions.erase(subIter);
				break;
			}
		}
	}
	//MEMBER VARIABLES/////////////////////////////////////////////////////////////////////
	std::vector<Subscription, UntrackedAllocator<Subscription>> m_subscriptions;
};