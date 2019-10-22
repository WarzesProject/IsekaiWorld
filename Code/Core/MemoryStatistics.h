#pragma once

// Thread safe class used for storing total number of memory allocations and deallocations, primarily for statistic purposes.
class MemoryCounter
{
public:
	static uint64_t GetNumAllocs()
	{
		return Allocs;
	}

	static uint64_t GetNumFrees()
	{
		return Frees;
	}

private:
	friend class MemoryAllocatorBase;

	static void incAllocCount() { ++Allocs; }
	static void incFreeCount() { ++Frees; }

	inline static SE_THREADLOCAL uint64_t Allocs = 0;
	inline static SE_THREADLOCAL uint64_t Frees = 0;
};