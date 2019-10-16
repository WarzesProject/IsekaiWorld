#pragma once

#define __EVENTLISTENERBODY(_class_)                                                        \
	public: virtual ~_class_() {if (m_register) *m_register = false; m_register = nullptr;} \
	public: void __register(bool &reg) {m_register = &reg;}                                 \
	private: bool *m_register = nullptr;                                                    \
	
class EventResizeEventListener
{
	__EVENTLISTENERBODY(EventResizeEventListener);
public:
	virtual void Resize(uint32_t w, uint32_t h) = 0;
};

class EventKeyListener
{
	__EVENTLISTENERBODY(EventKeyListener);
public:
	virtual void Key(uint32_t key, bool press) = 0;
};

template<typename T, typename ... Args>
class EventSignal
{
	struct listData
	{
		void operator()(Args ...args) { (listener->*func)(args ...); }

		typedef void(T::*Func)(Args ...);
		T *listener = nullptr;
		Func func;
		bool active = false;		
	};

public:

	template <typename TypeU>
	void Connect(TypeU *listener, void(TypeU::*func)(Args ...))
	{
		if (!listener) return;
		listData data = { (T*)listener, (void(T::*)(Args...))func, true };
		m_lists.emplace_back(data);

		auto &it = m_lists.back();
		listener->T::__register(it.active);
	}

	void operator()(Args ...args) noexcept
	{
		for (auto i = m_lists.begin(); i != m_lists.end();)
		{
			if ((*i).active)
			{
				(*i)(args...);
				++i;
			}
			else
				i = m_lists.erase(i);
		}
	}

private:
	std::list<listData> m_lists;
};