#pragma once

#include "MemoryAllocator.h"

// Describes a memory stack of a certain block capacity. See MemStack for more information.
// BlockCapacity Minimum size of a block. Larger blocks mean less memory allocations, but also potentially more wasted memory. If an allocation requests more bytes than BlockCapacity, first largest multiple is used instead.
template <int BlockCapacity = 1024 * 1024>
class MemStackInternal
{
private:
	// A single block of memory of BlockCapacity size. A pointer to the first free address is stored, and a remaining size.
	class MemBlock
	{
	public:
		MemBlock(size_t nsize) : mSize(nsize) {}

		// Returns the first free address and increments the free pointer. Caller needs to ensure the remaining block size is adequate before calling.
		uint8_t* Alloc(uint32_t amount)
		{
			uint8_t *freePtr = &mData[mFreePtr];
			mFreePtr += amount;
			return freePtr;
		}

		// Deallocates the provided pointer. Deallocation must happen in opposite order from allocation otherwise corruption will occur.
		// Pointer to @p data isn't actually needed, but is provided for debug purposes in order to more easily track out-of-order deallocations.
		void Dealloc(uint8_t *data, uint32_t amount)
		{
			mFreePtr -= amount;
			SE_ASSERT((&mData[mFreePtr]) == data && "Out of order stack deallocation detected. Deallocations need to happen in order opposite of allocations.");
		}

		uint8_t *mData = nullptr;
		uint32_t mFreePtr = 0;
		uint32_t mSize = 0;
		MemBlock *mNextBlock = nullptr;
		MemBlock *mPrevBlock = nullptr;
	};

public:
	MemStackInternal()
	{
		mFreeBlock = allocBlock(BlockCapacity);
	}

	~MemStackInternal()
	{
		SE_ASSERT(mFreeBlock->mFreePtr == 0 && "Not all blocks were released before shutting down the stack allocator.");

		MemBlock* curBlock = mFreeBlock;
		while (curBlock != nullptr)
		{
			MemBlock* nextBlock = curBlock->mNextBlock;
			deallocBlock(curBlock);

			curBlock = nextBlock;
		}
	}

	/**
	 * Allocates the given amount of memory on the stack.
	 *
	 * @param[in]	amount	The amount to allocate in bytes.
	 *
	 * @note
	 * Allocates the memory in the currently active block if it is large enough, otherwise a new block is allocated.
	 * If the allocation is larger than default block size a separate block will be allocated only for that allocation,
	 * making it essentially a slower heap allocator.
	 * @note
	 * Each allocation comes with a 4 byte overhead.
	 */
	uint8_t* alloc(uint32_t amount)
	{
		amount += sizeof(uint32_t);

		uint32_t freeMem = mFreeBlock->mSize - mFreeBlock->mFreePtr;
		if (amount > freeMem)
			allocBlock(amount);

		uint8_t* data = mFreeBlock->Alloc(amount);

		uint32_t* storedSize = reinterpret_cast<uint32_t*>(data);
		*storedSize = amount;

		return data + sizeof(uint32_t);
	}

	/** Deallocates the given memory. Data must be deallocated in opposite order then when it was allocated. */
	void dealloc(uint8_t* data)
	{
		data -= sizeof(uint32_t);

		uint32_t* storedSize = reinterpret_cast<uint32_t*>(data);
		mFreeBlock->Dealloc(data, *storedSize);

		if (mFreeBlock->mFreePtr == 0)
		{
			MemBlock* emptyBlock = mFreeBlock;

			if (emptyBlock->mPrevBlock != nullptr)
				mFreeBlock = emptyBlock->mPrevBlock;

			// Merge with next block
			if (emptyBlock->mNextBlock != nullptr)
			{
				uint32_t totalSize = emptyBlock->mSize + emptyBlock->mNextBlock->mSize;

				if (emptyBlock->mPrevBlock != nullptr)
					emptyBlock->mPrevBlock->mNextBlock = nullptr;
				else
					mFreeBlock = nullptr;

				deallocBlock(emptyBlock->mNextBlock);
				deallocBlock(emptyBlock);

				allocBlock(totalSize);
			}
		}
	}

private:
	// Allocates a new block of memory using a heap allocator. Block will never be smaller than BlockCapacity no matter the wantedSize.
	MemBlock* allocBlock(uint32_t wantedSize)
	{
		uint32_t blockSize = BlockCapacity;
		if (wantedSize > blockSize)
			blockSize = wantedSize;

		MemBlock* newBlock = nullptr;
		MemBlock* curBlock = mFreeBlock;

		while (curBlock != nullptr)
		{
			MemBlock* nextBlock = curBlock->mNextBlock;
			if (nextBlock != nullptr && nextBlock->mSize >= blockSize)
			{
				newBlock = nextBlock;
				break;
			}

			curBlock = nextBlock;
		}

		if (newBlock == nullptr)
		{
			uint8_t *data = (uint8_t*)reinterpret_cast<uint8_t*>(se_alloc(blockSize + sizeof(MemBlock)));
			newBlock = new (data)MemBlock(blockSize);
			data += sizeof(MemBlock);

			newBlock->mData = data;
			newBlock->mPrevBlock = mFreeBlock;

			if (mFreeBlock != nullptr)
			{
				if (mFreeBlock->mNextBlock != nullptr)
					mFreeBlock->mNextBlock->mPrevBlock = newBlock;

				newBlock->mNextBlock = mFreeBlock->mNextBlock;
				mFreeBlock->mNextBlock = newBlock;
			}
		}

		mFreeBlock = newBlock;
		return newBlock;
	}

	void deallocBlock(MemBlock *block)
	{
		block->~MemBlock();
		se_free(block);
	}

	MemBlock *mFreeBlock = nullptr;
};

/**
* One of the fastest, but also very limiting type of allocator. All deallocations must happen in opposite order from
* allocations.
*
* @note
* It's mostly useful when you need to allocate something temporarily on the heap, usually something that gets
* allocated and freed within the same method.
* @note
* Each allocation comes with a pretty hefty 4 byte memory overhead, so don't use it for small allocations.
* @note
* Thread safe. But you cannot allocate on one thread and deallocate on another. Threads will keep
* separate stacks internally. Make sure to call beginThread()/endThread() for any thread this stack is used on.
*/
class MemStack
{
public:
	/**
	 * Sets up the stack with the currently active thread. You need to call this on any thread before doing any allocations or deallocations.
	 */
	static void BeginThread();

	/**
	 * Cleans up the stack for the current thread. You may not perform any allocations or deallocations after this is called, unless you call beginThread again.
	 */
	static void EndThread();

	/** @copydoc MemStackInternal::alloc() */
	static uint8_t* Alloc(uint32_t amount);

	/** @copydoc MemStackInternal::dealloc() */
	static void DeallocLast(uint8_t* data);

private:
	static SE_THREADLOCAL MemStackInternal<1024 * 1024> * ThreadMemStack;
};