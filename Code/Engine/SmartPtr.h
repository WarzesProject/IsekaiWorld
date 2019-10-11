#pragma once

// Reference count structure. Used in both intrusive and non-intrusive reference counting.
struct RefCount
{
	// Number of strong references. These keep the object alive.
	unsigned refs = 0;
	// Number of weak references.
	unsigned weakRefs = 0;
	// Expired flag. The object is no longer safe to access after this is set true.
	bool expired = false;
};

// Base class for intrusively reference counted objects that can be pointed to with SharedPtr and WeakPtr. These are not copy-constructible and not assignable.
class RefCounted
{
public:
	RefCounted() = default;
	virtual ~RefCounted();

	// Add a strong reference. Allocate the reference count structure first if necessary.
	void AddRef();
	// Release a strong reference. Destroy the object when the last strong reference is gone.
	void ReleaseRef();

	// Return the number of strong references.
	unsigned Refs() const { return refCount ? refCount->refs : 0; }
	// Return the number of weak references.
	unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
	// Return pointer to the reference count structure. Allocate if not allocated yet.
	RefCount* RefCountPtr();

private:
	RefCounted(const RefCounted&) = delete;
	RefCounted& operator=(const RefCounted&) = delete;

	RefCount *refCount = nullptr;
};

template <class T> 
class WeakPtr;

template <class T> 
class SharedPtr
{
public:
	SharedPtr() = default;

	SharedPtr(const SharedPtr<T>& ptr_) :
		ptr(nullptr)
	{
		*this = ptr_;
	}

	SharedPtr(T* ptr_) :
		ptr(nullptr)
	{
		*this = ptr_;
	}

	~SharedPtr()
	{
		Reset();
	}

	SharedPtr<T>& operator = (T* rhs)
	{
		if (Get() == rhs)
			return *this;

		Reset();
		ptr = rhs;
		if (ptr)
			ptr->AddRef();
		return *this;
	}

	SharedPtr<T>& operator = (const SharedPtr<T>& rhs)
	{
		if (*this == rhs)
			return *this;

		Reset();
		ptr = rhs.ptr;
		if (ptr)
			ptr->AddRef();
		return *this;
	}

	void Reset()
	{
		if (ptr)
		{
			ptr->ReleaseRef();
			ptr = nullptr;
		}
	}

	template <class U> void StaticCast(const SharedPtr<U>& rhs)
	{
		*this = static_cast<T*>(rhs.ptr);
	}

	template <class U> void DynamicCast(const WeakPtr<U>& rhs)
	{
		Reset();
		T* rhsObject = dynamic_cast<T*>(rhs.ptr);
		if (rhsObject)
			*this = rhsObject;
	}

	bool operator == (const SharedPtr<T>& rhs) const { return ptr == rhs.ptr; }
	bool operator == (T* rhs) const { return ptr == rhs; }
	bool operator != (const SharedPtr<T>& rhs) const { return !(*this == rhs); }
	bool operator != (T* rhs) const { return !(*this == rhs); }

	T* operator -> () const { assert(ptr); return ptr; }
	T& operator * () const { assert(ptr); return ptr; }
	operator T* () const { return ptr; }

	T* Get() const { return ptr; }
	unsigned Refs() const { return ptr ? ptr->Refs() : 0; }
	unsigned WeakRefs() const { return ptr ? ptr->WeakRefs() : 0; }
	bool IsNull() const { return ptr == nullptr; }

private:
	T *ptr = nullptr;
};

template <class T, class U> SharedPtr<T> StaticCast(const SharedPtr<U>& rhs)
{
	SharedPtr<T> ret;
	ret.StaticCast(rhs);
	return ret;
}

template <class T, class U> SharedPtr<T> DynamicCast(const SharedPtr<U>& rhs)
{
	SharedPtr<T> ret;
	ret.DynamicCast(rhs);
	return ret;
}

// Pointer which holds a weak reference to a RefCounted subclass. Can track destruction but does not keep the object alive.
template <class T> 
class WeakPtr
{
public:
	WeakPtr() = default;

	WeakPtr(const WeakPtr<T>& ptr_) :
		ptr(nullptr),
		refCount(nullptr)
	{
		*this = ptr_;
	}

	WeakPtr(const SharedPtr<T>& ptr_) :
		ptr(nullptr),
		refCount(nullptr)
	{
		*this = ptr_;
	}

	WeakPtr(T* ptr_) :
		ptr(nullptr),
		refCount(nullptr)
	{
		*this = ptr_;
	}

	~WeakPtr()
	{
		Reset();
	}

	WeakPtr<T>& operator = (const WeakPtr<T>& rhs)
	{
		if (*this == rhs)
			return *this;

		Reset();
		ptr = rhs.ptr;
		refCount = rhs.refCount;
		if (refCount)
			++(refCount->weakRefs);
		return *this;
	}

	WeakPtr<T>& operator = (const SharedPtr<T>& rhs)
	{
		if (*this == rhs)
			return *this;

		Reset();
		ptr = rhs.Get();
		refCount = ptr ? ptr->RefCountPtr() : nullptr;
		if (refCount)
			++(refCount->weakRefs);
		return *this;
	}

	WeakPtr<T>& operator = (T* rhs)
	{
		if (Get() == rhs)
			return *this;

		Reset();
		ptr = rhs;
		refCount = ptr ? ptr->RefCountPtr() : nullptr;
		if (refCount)
			++(refCount->weakRefs);
		return *this;
	}

	void Reset()
	{
		if (refCount)
		{
			--(refCount->weakRefs);
			// If expired and no more weak references, destroy the reference count
			if (refCount->expired && refCount->weakRefs == 0)
				delete refCount;
			ptr = nullptr;
			refCount = nullptr;
		}
	}

	template <class U> void StaticCast(const WeakPtr<U>& rhs)
	{
		*this = static_cast<T*>(rhs.ptr);
	}

	template <class U> void DynamicCast(const WeakPtr<U>& rhs)
	{
		Reset();
		T* rhsObject = dynamic_cast<T*>(rhs.ptr);
		if (rhsObject)
			*this = rhsObject;
	}

	bool operator == (const WeakPtr<T>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
	bool operator == (const SharedPtr<T>& rhs) const { return ptr == rhs.Get(); }
	bool operator == (T* rhs) const { return ptr == rhs; }
	bool operator != (const WeakPtr<T>& rhs) const { return !(*this == rhs); }
	bool operator != (const SharedPtr<T>& rhs) const { return !(*this == rhs); }
	bool operator != (T* rhs) const { return !(*this == rhs); }

	T* operator -> () const { T* ret = Get(); assert(ret); return ret; }
	T& operator * () const { T* ret = Get(); assert(ret); return ret; }
	operator T* () const { return Get(); }

	T* Get() const
	{
		if (refCount && !refCount->expired)
			return ptr;
		else
			return nullptr;
	}

	unsigned Refs() const { return refCount ? refCount->refs : 0; }
	unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
	bool IsNull() const { return ptr == nullptr; }
	bool IsExpired() const { return refCount && refCount->expired; }

private:
	T *ptr = nullptr;
	RefCount *refCount = nullptr;
};

template <class T, class U> WeakPtr<T> StaticCast(const WeakPtr<U>& rhs)
{
	WeakPtr<T> ret;
	ret.StaticCast(rhs);
	return ret;
}

template <class T, class U> WeakPtr<T> DynamicCast(const WeakPtr<U>& rhs)
{
	WeakPtr<T> ret;
	ret.DynamicCast(rhs);
	return ret;
}

// Pointer which holds a strong reference to an array and allows shared ownership. Uses non-intrusive reference counting.
template <class T> 
class SharedArrayPtr
{
public:
	SharedArrayPtr() = default;

	SharedArrayPtr(const SharedArrayPtr<T>& ptr_) :
		ptr(nullptr),
		refCount(nullptr)
	{
		*this = ptr_;
	}

	explicit SharedArrayPtr(T* ptr_) :
		ptr(nullptr),
		refCount(nullptr)
	{
		*this = ptr_;
	}

	~SharedArrayPtr()
	{
		Reset();
	}

	SharedArrayPtr<T>& operator = (const SharedArrayPtr<T>& rhs)
	{
		if (*this == rhs)
			return *this;

		Reset();
		ptr = rhs.ptr;
		refCount = rhs.refCount;
		if (refCount)
			++(refCount->refs);

		return *this;
	}

	SharedArrayPtr<T>& operator = (T* rhs)
	{
		if (Get() == rhs)
			return *this;

		Reset();

		if (rhs)
		{
			ptr = rhs;
			refCount = new RefCount();
			if (refCount)
				++(refCount->refs);
		}

		return *this;
	}

	T* operator -> () const { assert(ptr); return ptr; }
	T& operator * () const { assert(ptr); return *ptr; }
	T& operator [] (size_t index) { assert(ptr); return ptr[index]; }
	const T& operator [] (size_t index) const { assert(ptr); return ptr[index]; }
	
	bool operator == (const SharedArrayPtr<T>& rhs) const { return ptr == rhs.ptr; }
	bool operator != (const SharedArrayPtr<T>& rhs) const { return !(*this == rhs); }

	operator bool() const { return ptr != nullptr; }

	void Reset()
	{
		if (refCount)
		{
			--(refCount->refs);
			if (refCount->refs == 0)
			{
				refCount->expired = true;
				delete[] ptr;
				// If no weak refs, destroy the ref count now too
				if (refCount->weakRefs == 0)
					delete refCount;
			}
		}

		ptr = 0;
		refCount = 0;
	}

	template <class U> void StaticCast(const SharedArrayPtr<U>& rhs)
	{
		Reset();
		ptr = static_cast<T*>(rhs.Get());
		refCount = rhs.RefCountPtr();
		if (refCount)
			++(refCount->refs);
	}

	template <class U> void ReinterpretCast(const SharedArrayPtr<U>& rhs)
	{
		Reset();
		ptr = reinterpret_cast<T*>(rhs.Get());
		refCount = rhs.RefCountPtr();
		if (refCount)
			++(refCount->refs);
	}

	T* Get() const { return ptr; }
	unsigned Refs() const { return refCount ? refCount->refs : 0; }
	unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
	RefCount* RefCountPtr() const { return refCount; }
	bool IsNull() const { return ptr == nullptr; }

private:
	template <class U> SharedArrayPtr<T>& operator = (const SharedArrayPtr<U>& rhs) = delete;

	T *ptr = nullptr;
	RefCount *refCount = nullptr;
};

template <class T, class U> SharedArrayPtr<T> StaticCast(const SharedArrayPtr<U>& ptr)
{
	SharedArrayPtr<T> ret;
	ret.StaticCast(ptr);
	return ret;
}

template <class T, class U> SharedArrayPtr<T> ReinterpretCast(const SharedArrayPtr<U>& ptr)
{
	SharedArrayPtr<T> ret;
	ret.ReinterpretCast(ptr);
	return ret;
}