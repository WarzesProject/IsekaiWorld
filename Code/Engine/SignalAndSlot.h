#pragma once

namespace _detail
{
	template<typename Return, typename ... Args>
	class BaseSlot
	{
	public:
		virtual ~BaseSlot() = default;
		virtual Return operator()(Args ...) const noexcept = 0;
	};

	template<class Type, typename Return, typename ... Args>
	class MethodSlot final : virtual public BaseSlot<Return, Args...>
	{
	public:
		typedef Return(Type::*Func)(Args ...);

		MethodSlot(Type &t, Func f) : m_t(&t), m_f(f) {}

		Return operator()(Args ...args) const noexcept final { return (m_t->*m_f)(args ...); }
	private:
		Type *m_t;
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

	template<class Type, typename Return, typename ... Args>
	void Connect(Type &t, Return(Type::*f)(Args ...))
	{
		auto *slot = new _detail::MethodSlot<Type, Return, Args ...>(t, f);
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
	using slotType = _detail::BaseSlot<void, Args...>;

	std::forward_list<slotType*> m_slots;
	bool m_isEmitting = false;
};