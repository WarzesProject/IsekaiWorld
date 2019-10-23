#include "stdafx.h"
#include "StackAlloc.h"
//-----------------------------------------------------------------------------
SE_THREADLOCAL MemStackInternal<1024 * 1024> *MemStack::ThreadMemStack = nullptr;
//-----------------------------------------------------------------------------
void MemStack::BeginThread()
{
	if (ThreadMemStack != nullptr)
		EndThread();

	ThreadMemStack = se_new<MemStackInternal<1024 * 1024>>();
}
//-----------------------------------------------------------------------------
void MemStack::EndThread()
{
	if (ThreadMemStack != nullptr)
	{
		se_delete(ThreadMemStack);
		ThreadMemStack = nullptr;
	}
}
//-----------------------------------------------------------------------------
uint8_t* MemStack::Alloc(uint32_t numBytes)
{
	SE_ASSERT(ThreadMemStack != nullptr && "Stack allocation failed. Did you call beginThread?");

	return ThreadMemStack->alloc(numBytes);
}
//-----------------------------------------------------------------------------
void MemStack::DeallocLast(uint8_t* data)
{
	SE_ASSERT(ThreadMemStack != nullptr && "Stack deallocation failed. Did you call beginThread?");
	ThreadMemStack->dealloc(data);
}
//-----------------------------------------------------------------------------