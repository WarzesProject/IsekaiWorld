#pragma once

namespace _detailEventSystem
{
	template<typename ... Args>
	class BaseFunc
	{
	public:
		virtual ~BaseFunc() = default;
		virtual void operator()(Args ...) = 0;
	};

	template<class Type, typename ... Args>
	class MethodFunc final : public BaseFunc<Args...>
	{
	public:
		typedef void(Type::*Func)(Args ...);

		MethodFunc(Type *t, Func f) : m_t(t), m_f(f) {}

		void operator()(Args ...args) final { (m_t->*m_f)(args ...); }
	private:
		Type *m_t;
		Func m_f;
	};

	template<typename ... Args>
	struct Data
	{
		BaseFunc<Args...> *event = nullptr;
		bool active = false;
	};

} // namespace _detailEventSystem

class EventListener
{
public:
	virtual ~EventListener()
	{
		if (m_reg) *m_reg = false; m_reg = nullptr;
	}

	void _register_(bool &reg)
	{
		m_reg = &reg;
	}
private:
	bool *m_reg = nullptr;
};

template<typename ... Args>
class EventSignal
{
public:
	template<class Type>
	void Connect(Type *t, void(Type::*f)(Args ...))
	{
		_detailEventSystem::Data<Args...> d = {
			new _detailEventSystem::MethodFunc<Type, Args ...>(t, f),
			true
		};
		m_lists.emplace_back(d);

		auto &it = m_lists.back();
		t->_register_(it.active);
	}

	void operator()(Args ...args) noexcept
	{
		for (auto i = m_lists.begin(); i != m_lists.end();)
		{
			if ((*i).active)
			{
				(*(*i).event)(args...);
				++i;
			}
			else
			{
				delete (*i).event;
				i = m_lists.erase(i);
			}				
		}
	}

private:
	using eventData = _detailEventSystem::Data<Args...>;
	std::list<eventData> m_lists;
};















//#define __EVENTLISTENERBODY(_class_)                                            \
//	public: virtual ~_class_() {if (_m_reg) *_m_reg = false; _m_reg = nullptr;} \
//	public: void __register(bool &reg) {_m_reg = &reg;}                         \
//	private: bool *_m_reg = nullptr;                                            \
//	
//class EventResizeEventListener
//{
//	__EVENTLISTENERBODY(EventResizeEventListener);
//public:
//	virtual void Resize(uint32_t w, uint32_t h) = 0;
//};
//
//class EventKeyListener
//{
//	__EVENTLISTENERBODY(EventKeyListener);
//public:
//	virtual void Key(uint32_t key, bool press) = 0;
//};
//
//template<typename T, typename ... Args>
//class EventSignal
//{
//	struct listData
//	{
//		void operator()(Args ...args) { (listener->*func)(args ...); }
//
//		typedef void(T::*Func)(Args ...);
//		T *listener = nullptr;
//		Func func;
//		bool active = false;		
//	};
//
//public:
//
//	template <typename TypeU>
//	void Connect(TypeU *listener, void(TypeU::*func)(Args ...))
//	{
//		if (!listener) return;
//		m_lists.emplace_back({ (T*)listener, (void(T::*)(Args...))func, true });
//
//		//auto &it = m_lists.back();
//		//listener->T::__register(it.active);
//	}
//
//	void operator()(Args ...args) noexcept
//	{
//		for (auto i = m_lists.begin(); i != m_lists.end();)
//		{
//			if ((*i).active)
//			{
//				(*i)(args...);
//				++i;
//			}
//			else
//				i = m_lists.erase(i);
//		}
//	}
//
//private:
//	std::list<listData> m_lists;
//};