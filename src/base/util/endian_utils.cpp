
#include <cstdint>

namespace mixr {
namespace base {

bool is_big_endian()
{
   union {
      std::uint32_t i;
      char c[4];
   } bint = { 0x01020304 };

   return bint.c[0] == 1;
}

}
}

