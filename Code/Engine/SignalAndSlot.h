#pragma once

namespace _detail
{
	template<typename ... Args>
	class BaseSlot
	{
	public:
		virtual ~BaseSlot() = default;

		virtual void operator()(Args ...)  = 0;
	};

	template<class Type, typename ... Args>
	class MethodSlot final : public BaseSlot<Args...>
	{
	public:
		typedef void(Type::*Func)(Args ...);

		MethodSlot(Type &t, Func f) : m_t(&t), m_f(f) {}

		void operator()(Args ...args) final { (m_t->*m_f)(args ...); }
	private:
		Type *m_t;
		Func m_f;
	};

	template<class Type, typename ... Args>
	class SmartMethodSlot final : public BaseSlot<Args...>
	{
	public:
		typedef void(Type::*Func)(Args ...);

		SmartMethodSlot(std::weak_ptr<Type> t, Func f) : m_t(t), m_f(f) {}

		void operator()(Args ...args)  final
		{ 
			auto smartPointer = m_t.lock();
			if (smartPointer)
			{
				auto ptr = smartPointer.get();
				(ptr->*m_f)(args ...);
			}		
		}
	private:
		std::weak_ptr<Type> m_t;
		Func m_f;
	};

} // namespace detail

template<typename ... Args>
class Signal
{
public:
	~Signal()
	{
		for (auto &i : m_slots)
			SafeDelete(i);
		m_slots.clear();
	}

	template<class Type, typename ... Args>
	void Connect(Type &t, void(Type::*f)(Args ...))
	{
		auto *slot = new _detail::MethodSlot<Type, Args ...>(t, f);
		m_slots.emplace_front(slot);
	}

	template<class Type, typename ... Args>
	void Connect(std::weak_ptr<Type> t, void(Type::*f)(Args ...))
	{
		auto *slot = new _detail::SmartMethodSlot<Type, Args...>(t, f);
		m_slots.emplace_front(slot);
	}	   

	void operator()(Args ...args) noexcept
	{
		if (m_isEmitting) return;
		m_isEmitting = true;
		for (auto &i : m_slots)
			(*i)(args...);
		m_isEmitting = false;
	}

private:
	using slotType = _detail::BaseSlot<Args...>;

	std::forward_list<slotType*> m_slots;
	bool m_isEmitting = false;
};