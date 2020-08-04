#ifndef __mixr_base_util_endian_utils_HPP__
#define __mixr_base_util_endian_utils_HPP__

#include <array>
#include <algorithm>

namespace mixr {
namespace base {

bool is_big_endian();

//-----------------------------------------------------------
// Template to swap bytes to/from big/little endian order
//
// source: Robust endianness conversion website
//-----------------------------------------------------------
template <typename T>
void swap_endian(T &val) {
   union U {
      T val;
      std::array<std::uint8_t, sizeof(T)> raw;
   } src, dst;

   src.val = val;
   std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
   val = dst.val;
}

}
}

#endif

