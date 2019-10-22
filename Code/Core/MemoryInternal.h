#pragma once

inline void* PlatformAlignedAlloc16(size_t size)
{
#if SE_PLATFORM_WINDOWS
	return _aligned_malloc(size, 16);
#elif SE_PLATFORM_LINUX || SE_PLATFORM_ANDROID
	return ::memalign(16, size);
#else
	return ::malloc(size);
#endif
}

inline void PlatformAlignedFree16(void *ptr)
{
#if SE_PLATFORM_WINDOWS
	_aligned_free(ptr);
#elif SE_PLATFORM_LINUX || SE_PLATFORM_ANDROID
	::free(ptr);
#else
	::free(ptr);
#endif
}

inline void* PlatformAlignedAlloc(size_t size, size_t alignment)
{
#if SE_PLATFORM_WINDOWS
	return _aligned_malloc(size, alignment);
#elif SE_PLATFORM_LINUX || SE_PLATFORM_ANDROID
	::memalign(alignment, size);
#else
	void *data = ::malloc(size + (alignment - 1) + sizeof(void*));
	if (data == nullptr)
		return nullptr;

	char *alignedData = ((char*)data) + sizeof(void*);
	alignedData += (alignment - ((uintptr_t)alignedData) & (alignment - 1)) & (alignment - 1);

	((void**)alignedData)[-1] = data;
	return alignedData;
#endif
}

inline void PlatformAlignedFree(void *ptr)
{
#if SE_PLATFORM_WINDOWS
	_aligned_free(ptr);
#elif SE_PLATFORM_LINUX || SE_PLATFORM_ANDROID
	::free(ptr);
#else
	::free(((void**)ptr)[-1]);
#endif
}