#pragma once

// Platform
#define SE_PLATFORM_ANDROID        0
#define SE_PLATFORM_EMSCRIPTEN     0
#define SE_PLATFORM_LINUX          0
#define SE_PLATFORM_WINDOWS        0
#define SE_PLATFORM_WINRT          0

//=============================================================================
// Platform
//=============================================================================
// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_WIN32) || defined(_WIN64)
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#	if defined(_MSC_VER)
#		include <winapifamily.h>
#	endif
#	if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#		undef  SE_PLATFORM_WINDOWS
#		define SE_PLATFORM_WINDOWS 1
#	else
#		undef  SE_PLATFORM_WINRT
#		define SE_PLATFORM_WINRT 1
#	endif
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#	include <sys/cdefs.h> // Defines __BIONIC__ and includes android/api-level.h
#	undef  SE_PLATFORM_ANDROID
#	define SE_PLATFORM_ANDROID __ANDROID_API__
#elif  defined(__linux__)
#	undef  SE_PLATFORM_LINUX
#	define SE_PLATFORM_LINUX 1
#elif defined(__EMSCRIPTEN__)
#	undef  SE_PLATFORM_EMSCRIPTEN
#	define SE_PLATFORM_EMSCRIPTEN 1
#endif