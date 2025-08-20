#pragma once
// this should probably have a better name than base? Core? CoreMinimal like UE? Should I only IWYU?
// Also some stuff should probably be private.

#include "Flibbert/Core/PlatformDetection.h"
#include "Flibbert/Core/Types.h"

// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) ||                        \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define FBT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define FBT_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
	#define FBT_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) ||                                  \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define FBT_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define FBT_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define FBT_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define FBT_FUNC_SIG __func__
#else
	#define FBT_FUNC_SIG "FBT_FUNC_SIG unknown!"
#endif

#ifndef NDEBUG
	#define FBT_ENABLE_ASSERTS
	#define FBT_DEBUG

	// See https://github.com/scottt/debugbreak
	#if defined(_MSC_VER)
		#define FBT_DEBUG_BREAK() (__nop(), __debugbreak())
	#elif defined(__clang__)
		#define FBT_DEBUG_BREAK() __builtin_debugtrap()
	#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
		#define FBT_DEBUG_BREAK() __asm__ volatile("int3;nop")
	#elif defined(__GNUC__) && defined(__thumb__)
		#define FBT_DEBUG_BREAK() __asm__ volatile(".inst 0xde01")
	#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
		#define FBT_DEBUG_BREAK() __asm__ volatile(".inst 0xe7f001f0")
	#else
		#define FBT_DEBUG_BREAK()
	#endif
#else
	#define FBT_DEBUG_BREAK()
#endif

#include "Flibbert/Core/Log.h"

#include "Flibbert/Core/AssertionMacros.h"

#include "Flibbert/Core/Delegates.h"
