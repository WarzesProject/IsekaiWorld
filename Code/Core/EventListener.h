#pragma once

class EventListener
{
	template<typename ... Args> friend class EventSignal;
public:
	virtual ~EventListener()
	{
		DisconnectAllEvents();
	}

	// отключает от всех событий
	void DisconnectAllEvents()
	{
		if (m_rem) *m_rem = true; m_rem = nullptr;
		currentData = nullptr;
	}

private:
	void registerEvent(void *curData, bool &rem)
	{
		currentData = curData;
		m_rem = &rem;
	}

	void *currentData = nullptr;
	bool *m_rem = nullptr;
};