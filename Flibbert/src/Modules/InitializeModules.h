#pragma once

namespace Flibbert::Modules {

void InitializeModules();

#if !FBT_MODULES_ENABLED
inline void InitializeModules() {}
#endif

}
