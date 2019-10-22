#pragma once

#include "MemoryInternal.h"
#include "MemoryStatistics.h"

// Base class all memory allocators need to inherit. Provides allocation and free counting.
class MemoryAllocatorBase
{
protected:
	static void incAllocCount() { MemoryCounter::incAllocCount(); }
	static void incFreeCount() { MemoryCounter::incFreeCount(); }
};

// Memory allocator providing a generic implementation. Specialize for specific categories as needed.
template<class T>
class MemoryAllocator : public MemoryAllocatorBase
{
public:
	static void* Allocate(size_t bytes)
	{
#if SE_PROFILING_ENABLED
		incAllocCount();
#endif
		return malloc(bytes);
	}

	static void* AllocateAligned(size_t bytes, size_t alignment)
	{
#if SE_PROFILING_ENABLED
		incAllocCount();
#endif
		return PlatformAlignedAlloc(bytes, alignment);
	}

	static void* AllocateAligned16(size_t bytes)
	{
#if SE_PROFILING_ENABLED
		incAllocCount();
#endif

		return PlatformAlignedAlloc16(bytes);
	}

	static void Free(void *ptr)
	{
#if SE_PROFILING_ENABLED
		incFreeCount();
#endif

		::free(ptr);
	}

	static void FreeAligned(void *ptr)
	{
#if SE_PROFILING_ENABLED
		incFreeCount();
#endif
		PlatformAlignedFree(ptr);
	}

	static void FreeAligned16(void *ptr)
	{
#if SE_PROFILING_ENABLED
		incFreeCount();
#endif

		PlatformAlignedFree16(ptr);
	}
};

// General allocator provided by the OS. Use for persistent long term allocations, and allocations that don't happen often.
class GenAlloc{};

template<class Alloc>
void* se_alloc(size_t count)
{
	return MemoryAllocator<Alloc>::Allocate(count);
}

template<class T, class Alloc>
T* se_alloc()
{
	return (T*)MemoryAllocator<Alloc>::Allocate(sizeof(T));
}

template<class T, class Alloc>
T* se_newN(size_t count)
{
	T *ptr = (T*)MemoryAllocator<Alloc>::Allocate(sizeof(T) * count);

	for (size_t i = 0; i < count; ++i)
		new (&ptr[i]) T;

	return ptr;
}

template<class Type, class Alloc, class... Args>
Type* se_new(Args &&...args)
{
	return new (se_alloc<Type, Alloc>()) Type(std::forward<Args>(args)...);
}

template<class Alloc>
void se_free(void *ptr)
{
	MemoryAllocator<Alloc>::Free(ptr);
}

template<class T, class Alloc = GenAlloc>
void se_delete(T *ptr)
{
	(ptr)->~T();
	MemoryAllocator<Alloc>::Free(ptr);
}

// Callable struct that acts as a proxy for se_delete
template<class T, class Alloc = GenAlloc>
struct Deleter
{
	constexpr Deleter() noexcept = default;

	// Constructor enabling deleter conversion and therefore polymorphism with smart points (if they use the same allocator).
	template <class T2, std::enable_if_t<std::is_convertible<T2*, T*>::value, int> = 0>
	constexpr Deleter(const Deleter<T2, Alloc> &other) noexcept {}

	void operator()(T* ptr) const
	{
		se_delete<T, Alloc>(ptr);
	}
};

template<class T, class Alloc = GenAlloc>
void se_deleteN(T *ptr, size_t count)
{
	for (size_t i = 0; i < count; ++i)
		ptr[i].~T();

	MemoryAllocator<Alloc>::Free(ptr);
}

inline void* se_alloc(size_t count)
{
	return MemoryAllocator<GenAlloc>::Allocate(count);
}

template<class T>
T* se_alloc()
{
	return (T*)MemoryAllocator<GenAlloc>::Allocate(sizeof(T));
}

inline void* se_alloc_aligned(size_t count, size_t align)
{
	return MemoryAllocator<GenAlloc>::AllocateAligned(count, align);
}

inline void* se_alloc_aligned16(size_t count)
{
	return MemoryAllocator<GenAlloc>::AllocateAligned16(count);
}

template<class T>
T* se_allocN(size_t count)
{
	return (T*)MemoryAllocator<GenAlloc>::Allocate(count * sizeof(T));
}

template<class T>
T* se_newN(size_t count)
{
	T *ptr = (T*)MemoryAllocator<GenAlloc>::Allocate(count * sizeof(T));

	for (size_t i = 0; i < count; ++i)
		new (&ptr[i]) T;

	return ptr;
}

template<class Type, class... Args>
Type* se_new(Args &&...args)
{
	return new (se_alloc<Type, GenAlloc>()) Type(std::forward<Args>(args)...);
}

inline void se_free(void* ptr)
{
	MemoryAllocator<GenAlloc>::Free(ptr);
}

inline void se_free_aligned(void *ptr)
{
	MemoryAllocator<GenAlloc>::FreeAligned(ptr);
}

inline void se_free_aligned16(void *ptr)
{
	MemoryAllocator<GenAlloc>::FreeAligned16(ptr);
}

template <class T, class Alloc = GenAlloc>
class StdAlloc
{
public:
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	constexpr StdAlloc() = default;
	constexpr StdAlloc(StdAlloc&&) = default;
	constexpr StdAlloc(const StdAlloc&) = default;
	template<class U, class Alloc2> constexpr StdAlloc(const StdAlloc<U, Alloc2>&) {};
	template<class U, class Alloc2> constexpr bool operator==(const StdAlloc<U, Alloc2>&) const noexcept { return true; }
	template<class U, class Alloc2> constexpr bool operator!=(const StdAlloc<U, Alloc2>&) const noexcept { return false; }

	template<class U> class rebind { public: using other = StdAlloc<U, Alloc>; };

	static T* allocate(const size_t num)
	{
		if (num == 0)
			return nullptr;

		if (num > max_size())
			return nullptr; // Error

		void* const pv = se_alloc<Alloc>(num * sizeof(T));
		if (!pv)
			return nullptr; // Error

		return static_cast<T*>(pv);
	}

	static void deallocate(pointer p, size_type)
	{
		se_free<Alloc>(p);
	}

	static constexpr size_t max_size() { return std::numeric_limits<size_type>::max() / sizeof(T); }
	static constexpr void destroy(pointer p) { p->~T(); }

	template<class... Args>
	static void construct(pointer p, Args&&... args) { new(p) T(std::forward<Args>(args)...); }
};