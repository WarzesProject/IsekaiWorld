#pragma once

#include "AutoPtr.h"
#include "SmartPtr.h"

class Event;

class EventHandler
{
public:
	EventHandler(RefCounted *receiver) : m_receiver(receiver) {};
	virtual ~EventHandler() = default;;

	virtual void Invoke(Event &event) = 0;

	RefCounted* Receiver() const { return m_receiver.Get(); }

protected:
	WeakPtr<RefCounted> m_receiver;
};

template <class T, class U> 
class EventHandlerImpl : public EventHandler
{
public:
	typedef void (T::*HandlerFunctionPtr)(U&);

	EventHandlerImpl(RefCounted *receiver, HandlerFunctionPtr function)
		: EventHandler(receiver)
		, m_function(function)
	{
		SE_ASSERT(m_function);
	}

	void Invoke(Event &event) override
	{
		T *typedReceiver = static_cast<T*>(receiver.Get());
		U &typedEvent = static_cast<U&>(event);
		(typedReceiver->*function)(typedEvent);
	}

private:
	HandlerFunctionPtr m_function;
};

// Notification and data passing mechanism, to which objects can subscribe by specifying a handler function. Subclass to include event-specific data.
class Event
{
public:
	Event() = default;
	virtual ~Event() = default;

	void Send(RefCounted *sender);
	// Subscribe to the event. The event takes ownership of the handler data. If there is already handler data for the same receiver, it is overwritten.
	void Subscribe(EventHandler *handler);
	void Unsubscribe(RefCounted *receiver);

	// Return whether has at least one valid receiver.
	bool HasReceivers() const;
	// Return whether has a specific receiver.
	bool HasReceiver(const RefCounted* receiver) const;
	// Return current sender.
	RefCounted* Sender() const { return m_currentSender; }

private:
	Event(const Event&) = delete;
	Event& operator = (const Event&) = delete;

	std::vector<AutoPtr<EventHandler>> m_handlers;
	WeakPtr<RefCounted> m_currentSender;
};