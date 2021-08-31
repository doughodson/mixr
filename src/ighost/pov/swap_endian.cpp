
#include <cstdint>

#include "mixr/base/util/endian_utils.hpp"

#include "mixr/ighost/pov/Pov.hpp"

namespace mixr {
namespace pov {

void swap_endian(Pov* const x)
{
   // position
   base::swap_endian<double>(x->north);
   base::swap_endian<double>(x->east);
   base::swap_endian<double>(x->alt_agl);

   // orientation
   base::swap_endian<double>(x->roll);
   base::swap_endian<double>(x->pitch);
   base::swap_endian<double>(x->heading);
}

}
}

