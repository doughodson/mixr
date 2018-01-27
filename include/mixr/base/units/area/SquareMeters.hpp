
#ifndef __mixr_base_SquareMeters_H__
#define __mixr_base_SquareMeters_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareMeters
// Description: An instance of SquareMeters with its value equal to 1.0 is one
//              base unit for area.
//------------------------------------------------------------------------------
class SquareMeters final: public Area
{
    DECLARE_SUBCLASS(SquareMeters, Area)

public:
    SquareMeters();
    SquareMeters(const double);
    SquareMeters(const Area&);

    static double convertStatic(const Area& n)              { return n.toArea(); }

    double toArea() const final                             { return getValue(); }
    double fromArea(const double a) const final             { return a; }
};

}
}

#endif
