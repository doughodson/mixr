
#ifndef __mixr_base_SquareMiles_H__
#define __mixr_base_SquareMiles_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareMiles
// Description: Square Meters * 0.00000038610216
//------------------------------------------------------------------------------
class SquareMiles final: public Area
{
    DECLARE_SUBCLASS(SquareMiles, Area)

public:
    SquareMiles();
    SquareMiles(const double);
    SquareMiles(const Area&);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SMI; }

    double toArea() const final                     { return getValue() * area::SMI2SM; }
    double fromArea(const double a) const final     { return a * area::SM2SMI; }
};

}
}

#endif
