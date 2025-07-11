#pragma once
// this should probably have a better name than base?

#define FBT_EXPAND_MACRO(x) x

#ifndef NDEBUG
	#define FBT_ENABLE_ASSERTS

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
