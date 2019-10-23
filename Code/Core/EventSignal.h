#pragma once

#include "EventListener.h"

namespace _detailEventSignal
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

		MethodFunc(Type *t, Func f) : m_type(t), m_func(f) {}

		void operator()(Args ...args) final { (m_type->*m_func)(args ...); }
	private:
		Type *m_type;
		Func m_func;
	};

	template<typename ... Args>
	struct DataFunc
	{
		BaseFunc<Args...> *eventFunc = nullptr;
		bool removeFunc = true;
		bool activeFunc = false;
	};

} // namespace _detailEventSignal

template<typename ... Args>
class EventSignal
{
	using eventData = _detailEventSignal::DataFunc<Args...>;
public:
	~EventSignal()
	{
		for (auto &i : m_lists)
			SafeDelete(i.eventFunc);
		m_lists.clear();
	}

	template<class Type>
	void Connect(Type *type, void(Type::*func)(Args ...))
	{
		_detailEventSignal::DataFunc<Args...> data =
		{
			new _detailEventSignal::MethodFunc<Type, Args ...>(type, func),
			false,
			true
		};
		m_lists.emplace_back(std::move(data));

		auto &it = m_lists.back();
		type->registerEvent(&it, it.removeFunc);
	}

	template<class Type>
	void Disconnect(Type *type)
	{
		auto data = static_cast<eventData*>(type->currentData);
		data->activeFunc = false;
	}
	
	void operator()(Args ...args) noexcept
	{
		for (auto i = m_lists.begin(); i != m_lists.end();)
		{
			if ((*i).activeFunc && !(*i).removeFunc)
			{
				(*(*i).eventFunc)(args...);
				++i;
			}
			else
			{
				delete (*i).eventFunc;
				i = m_lists.erase(i);
			}
		}
	}

private:	
	std::list<eventData> m_lists;
};