// Platform detection using predefined macros
#ifdef _WIN64
	#define FBT_PLATFORM_WINDOWS
#elifdef __linux__
	#define FBT_PLATFORM_LINUX
#else
	#error "Unsupported platform!"
#endif // End of platform detection

#if defined(FBT_PLATFORM_WINDOWS) || defined(FBT_PLATFORM_LINUX) || defined(FBT_PLATFORM_MACOS)
	#define FBT_PLATFORM_DESKTOP
#endif
