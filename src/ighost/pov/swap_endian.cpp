
#include <cstdint>

#include "mixr/base/util/endian_utils.hpp"

#include "mixr/ighost/pov/Pov.hpp"

namespace mixr {
namespace pov {

void swap_endian(Pov* const x)
{
   // position
   base::swap_endian<double>(x->latitude);
   base::swap_endian<double>(x->longitude);
   base::swap_endian<double>(x->altitude);
   // orientation
   base::swap_endian<double>(x->theta);
   base::swap_endian<double>(x->phi);
   base::swap_endian<double>(x->psi);
}

}
}

