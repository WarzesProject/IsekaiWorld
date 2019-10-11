#pragma once

template <class T> 
class AutoPtr
{
public:
	AutoPtr() = default;
	AutoPtr(const AutoPtr<T> &ptr) : m_ptr(ptr.m_ptr)
	{
		const_cast<AutoPtr<T>&>(ptr).m_ptr = nullptr;
	}
	AutoPtr(T *ptr) :m_ptr(ptr) {}
	~AutoPtr() { SafeDelete(m_ptr); }

	AutoPtr<T>& operator=(const AutoPtr<T> &rhs)
	{
		delete m_ptr;
		m_ptr = rhs.m_ptr;
		const_cast<AutoPtr<T>&>(rhs).m_ptr = nullptr;
		return *this;
	}

	AutoPtr<T>& operator=(T *rhs)
	{
		delete m_ptr;
		m_ptr = rhs;
		return *this;
	}

	T* Detach()
	{
		T *ret = m_ptr;
		m_ptr = nullptr;
		return ret;
	}

	void Reset()
	{
		*this = nullptr;
	}

	T* operator->() const { SE_ASSERT(m_ptr); return m_ptr; }
	T& operator*() const { SE_ASSERT(m_ptr); return *m_ptr; }
	operator T*() const { return m_ptr; }

	T* Get() const { return m_ptr; }
	bool IsNull() const { return m_ptr == nullptr; }

private:
	T *m_ptr = nullptr;
};