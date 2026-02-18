#pragma once

#if FBT_PROFILING_ENABLED
	#include "glad.h"
	#include "tracy/TracyOpenGL.hpp"

	// Call once after GL context creation
	#define FBT_PROFILE_GPU_INIT() TracyGpuContext

	// Profile a GPU scope — times the GL commands issued within it
	#define FBT_PROFILE_GPU_SCOPE(name) TracyGpuZone(name)

	// Call once per frame to provide a screenshot to tracy
	#define FBT_PROFILE_GPU_IMAGE(image, width, height, offset, flip) FrameImage(image, width, height, offset, flip)

	// Call once per frame AFTER SwapBuffers to collect GPU timing results
	#define FBT_PROFILE_GPU_COLLECT() TracyGpuCollect
#else
	#define FBT_PROFILE_GPU_INIT()
	#define FBT_PROFILE_GPU_SCOPE(name)
	#define FBT_PROFILE_GPU_IMAGE(image, width, height, offset, flip)
	#define FBT_PROFILE_GPU_COLLECT()
#endif
