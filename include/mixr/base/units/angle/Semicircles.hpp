
#ifndef __mixr_base_Semicircles_H__
#define __mixr_base_Semicircles_H__

#include "mixr/base/units/angle/Angle.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Semicircles
// Description:  An instance of Semicircles with its value equal to 1.0 is
//               one base unit for distances.
//------------------------------------------------------------------------------
class Semicircles final: public Angle
{
    DECLARE_SUBCLASS(Semicircles, Angle)

public:
    Semicircles();
    Semicircles(const double);
    Semicircles(const Angle&);

    static double convertStatic(const Angle& n)      { return n.toAngle(); }

    double toAngle() const final                     { return getValue(); }
    double fromAngle(const double a) const final     { return a; }
};

}
}

#endif
