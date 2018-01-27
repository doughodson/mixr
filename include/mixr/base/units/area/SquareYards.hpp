
#ifndef __mixr_base_SquareYards_H__
#define __mixr_base_SquareYards_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareYards
// Description: Square Meters * 1.19599
//------------------------------------------------------------------------------
class SquareYards final: public Area
{
    DECLARE_SUBCLASS(SquareYards, Area)

public:
    SquareYards();
    SquareYards(const double);
    SquareYards(const Area&);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SYD; }

    double toArea() const final                     { return getValue() * area::SYD2SM; }
    double fromArea(const double a) const final     { return a * area::SM2SYD; }
};

}
}

#endif
