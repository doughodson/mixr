
#ifndef __mixr_base_Degrees_H__
#define __mixr_base_Degrees_H__

#include "mixr/base/units/angle/Angle.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Degrees
// Description: Angle * 180.0
//------------------------------------------------------------------------------
class Degrees final: public Angle
{
    DECLARE_SUBCLASS(Degrees, Angle)

public:
    Degrees();
    Degrees(const double);
    Degrees(const Angle&);

    static double convertStatic(const Angle& n)           { return n.toAngle() * angle::SC2D; }

    double toAngle() const final                          { return getValue() * angle::D2SC; }
    double fromAngle(const double a) const final          { return a * angle::SC2D; }
};

}
}

#endif

