
#ifndef __mixr_base_util_atomics_H__
#define __mixr_base_util_atomics_H__

#if defined(WIN32)
  #if defined(__MINGW32__)
    // MinGW
    #include "mixr/base/util/platform/atomics_mingw.hpp"
  #else
    // Visual Studio
    #include "mixr/base/util/platform/atomics_msvc.hpp"
  #endif
#else
  // Linux GCC
  #include "mixr/base/util/platform/atomics_linux.hpp"
#endif

#endif

