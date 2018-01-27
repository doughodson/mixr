
#ifndef __mixr_base_Angle_H__
#define __mixr_base_Angle_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Angle
// Description:  Base class for angles.  Defined as a semicircle which is
//               equivalent to an instance of Semicircle with its value equal
//               to 1.0.
//
// Public methods (Defined in Angle, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Angle derived instance with a double.
//
//     set(const Angle& n)
//        Sets, and converts if necessary, an Angle derived instance with
//        another Angle derived instance.
//
//     double convert(const Angle& n)
//        Converts the value of an Angle derived instance into
//        the units of another Angle derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Angle& n)
//        Sends "( <the Angle derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Degrees, Radians, Semicircles):
//
//     double convertStatic(const Angle& n)
//        Static function to convert the given Angle derived
//        instance into the units of a specific Angle derived class.
//
//------------------------------------------------------------------------------
class Angle : public Unit
{
    DECLARE_SUBCLASS(Angle, Unit)

public:
    Angle();
    Angle(const double);

    void set(const double v)             { setValue(v); }
    void set(const Angle& n)             { setValue(fromAngle(n.toAngle())); }

    double convert(const Angle& n)       { return fromAngle(n.toAngle()); }

    virtual double toAngle() const = 0;
    virtual double fromAngle(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Angle& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif

