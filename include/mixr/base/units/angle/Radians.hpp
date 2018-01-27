
#ifndef __mixr_base_Radians_H__
#define __mixr_base_Radians_H__

#include "mixr/base/units/angle/Angle.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Radians
// Description: Angle * PI
//------------------------------------------------------------------------------
class Radians final: public Angle
{
    DECLARE_SUBCLASS(Radians, Angle)

public:
    Radians();
    Radians(const double);
    Radians(const Angle&);

    static double convertStatic(const Angle& n)      { return n.toAngle() * angle::SC2R; }

    double toAngle() const final                     { return getValue() * angle::R2SC; }
    double fromAngle(const double a) const final     { return a * angle::SC2R; }
};

}
}

#endif

