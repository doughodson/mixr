
#include <cstdint>

#include "mixr/base/util/endian_utils.hpp"

#include "mixr/ighost/pov/Pov.hpp"

namespace mixr {
namespace pov {

void swap_endian(Pov* const x)
{
   // position
   base::swap_endian<double>(x->x);
   base::swap_endian<double>(x->y);
   base::swap_endian<double>(x->z);
   // orientation
   base::swap_endian<double>(x->theta);
   base::swap_endian<double>(x->phi);
   base::swap_endian<double>(x->psi);
}

}
}

