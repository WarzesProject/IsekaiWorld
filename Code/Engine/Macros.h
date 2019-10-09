#pragma once

//=============================================================================
// Namespace
//=============================================================================
#define SE_NAMESPACE                   se
#define SE_NAMESPACE_BEGIN             namespace SE_NAMESPACE {
#define SE_NAMESPACE_END               }
namespace SE_NAMESPACE {}

//=============================================================================
// Platform Macros
//=============================================================================
#if SE_COMPILER_MSVC
#	define SE_FORCE_INLINE __forceinline
#	define SE_NO_VTABLE __declspec(novtable)	// https://habr.com/ru/post/442340/
#endif

//=============================================================================
// Macros Assert
//=============================================================================
#if SE_DEBUG
#	if SE_COMPILER_MSVC
#		define SE_ASSERT(expr) _ASSERT_EXPR((expr), nullptr)
#		define SE_ASSERT_MSG(expr, msg) _ASSERT_EXPR((expr), _CRT_WIDE(msg))
#	else
#		define SE_ASSERT(expr) assert((expr))
#		define SE_ASSERT_MSG(expr, msg) assert((expr) && (msg))
#	endif
#else
#	define SE_ASSERT(expr) ((void)0)
#	define SE_ASSERT_MSG(expr, msg) ((void)0)
#endif


//=============================================================================
// Macros Utility
//=============================================================================

#define SE_STRINGIZE( _n ) SE_STRINGIZE_2( _n )
#define SE_STRINGIZE_2( _n ) #_n

//=============================================================================
// Macros TODO
//=============================================================================
#if SE_COMPILER_MSVC
#	define TODO( _msg ) __pragma(message("" __FILE__ "(" SE_STRINGIZE(__LINE__) "): TODO: " _msg))
#else
#	define TODO( _msg )
#endif

