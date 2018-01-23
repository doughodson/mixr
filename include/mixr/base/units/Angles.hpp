//------------------------------------------------------------------------------
// Classes: Angle, Degrees, Radians, Semicircles
//------------------------------------------------------------------------------

#ifndef __mixr_base_Angles_H__
#define __mixr_base_Angles_H__

#include "mixr/base/units/Unit.hpp"

#include "mixr/base/util/constants.hpp"
#include "mixr/base/units/angle_utils.hpp"

#include <cmath>

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

    void set(const double v)             { val = v; }
    void set(const Angle& n)             { val = fromAngle(n.toAngle()); }

    double convert(const Angle& n)       { return fromAngle(n.toAngle()); }

    virtual double toAngle() const = 0;
    virtual double fromAngle(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Angle& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }


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

    double toAngle() const final                          { return static_cast<double>(val * angle::D2SC); }
    double fromAngle(const double a) const final          { return a * angle::SC2D; }
};

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

    double toAngle() const final                     { return static_cast<double>(val * angle::R2SC); }
    double fromAngle(const double a) const final     { return a * angle::SC2R; }
};

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

    double toAngle() const final                     { return static_cast<double>(val); }
    double fromAngle(const double a) const final     { return a; }
};

}
}

#endif

