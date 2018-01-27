
#ifndef __mixr_base_Inches_H__
#define __mixr_base_Inches_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Inches
// Description:  Meters / 0.0254
//------------------------------------------------------------------------------
class Inches final: public Distance
{
    DECLARE_SUBCLASS(Inches, Distance)

public:
    Inches();
    Inches(const double);
    Inches(const Distance&);

    static double convertStatic(const Distance& n)      { return n.toDistance() * distance::M2IN; }

    double toDistance() const final                     { return getValue() * distance::IN2M; }
    double fromDistance(const double a) const final     { return a * distance::M2IN; }
};

}
}

#endif
