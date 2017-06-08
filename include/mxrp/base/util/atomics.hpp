
#ifndef __mxrp_base_util_atomics_H__
#define __mxrp_base_util_atomics_H__

#if defined(WIN32)
  #if defined(__MINGW32__)
    // MinGW
    #include "mxrp/base/util/platform/atomics_mingw.hpp"
  #else
    // Visual Studio
    #include "mxrp/base/util/platform/atomics_msvc.hpp"
  #endif
#else
  // Linux GCC
  #include "mxrp/base/util/platform/atomics_linux.hpp"
#endif

#endif

