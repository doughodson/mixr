
#ifndef __mixr_base_Volume_H__
#define __mixr_base_Volume_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Volume
// Description:  Base class for volume.  Defined as a cubic meter which is
// equivalent to an instance of CubicMeters with its value equal to 1.0.
//
//
// Public methods (Defined in Volume, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Volume derived instance with an double.
//
//     set(const Volume& n)
//        Sets, and converts if necessary, a Volume derived
//        instance with another Volume derived instance.
//
//     double convert(const Volume& n)
//        Converts the value of a Volume derived instance into
//        the units of another Volume derived instance.
//
//   Output stream operator:<<
//        ostream& operator<<(ostream& sout, const Volume& n)
//        Sends "( <the Volume derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes: CubicMeters, CubicFeet, CubicInches, Liters):
//    double convertStatic(const Volume &n)
//    Static function to convert the given Volume derived instance
//    into the units of a specific Volume derived class.
//
//------------------------------------------------------------------------------
class Volume : public Unit
{
    DECLARE_SUBCLASS(Volume, Unit)

public:
    Volume();
    Volume(const double);

    void set(const double v)                 { setValue(v); }
    void set(const Volume& n)                { setValue(fromVolume(n.toVolume())); }

    double convert(const Volume& n) const    { return fromVolume(n.toVolume()); }

    virtual double toVolume() const = 0;
    virtual double fromVolume(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Volume& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
