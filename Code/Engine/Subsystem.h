#pragma once

#include "Error.h"

template <typename T>
class Subsystem
{
public:
	template<class ...Args>
	static bool Create(Args&& ...args)
	{
		if (IsValid())
		{
			CriticalErrorExit("Trying to create an already created module.");
			return false;
		}

		instance() = new T(std::forward<Args>(args)...);
		return IsValid();
	}

	template<typename SubClass, class ...Args>
	static bool Create(Args&& ...args)
	{
		if (IsValid())
		{
			CriticalErrorExit("Trying to create an already created module.");
			return false;
		}

		instance() = static_cast<T*>(new SubClass(std::forward<Args>(args)...));
		return IsValid();
	}

	static void Destroy()
	{
		SafeDelete(instance());
		valid() = false;
	}

	static T& Get()
	{
		if (!IsValid())
			CriticalErrorExit("Trying to access a module but it hasn't been created up yet.");

		return *instance();
	}

	static const bool IsValid()
	{
		return valid();
	}

protected:
	Subsystem() = default;
	~Subsystem() = default;

	static T*& instance()
	{
		static T* inst = nullptr;
		return inst;
	}

	static bool& valid()
	{
		static bool inst = false;
		return inst;
	}

	static void setValid(bool bvalid)
	{
		valid() = bvalid;
	}

private:
	Subsystem(Subsystem<T>&&) = delete;
	Subsystem(const Subsystem<T>&) = delete;
	Subsystem<T>& operator=(Subsystem<T>&&) = delete;
	Subsystem<T>& operator=(const Subsystem<T>&) = delete;
};

template <typename T>
inline T& GetSubsystem()
{
	return T::Get();
}