
#ifndef __mixr_base_Distance_H__
#define __mixr_base_Distance_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Distance
// Description:  Base class for distances.  Defined as a meter which is
//               equivalent to an instance of Meters with its value equal
//               to 1.0.
//
//
// Public methods (Defined in Distance, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Distance derived instance with an double.
//
//     set(const Distance& n)
//        Sets, and converts if necessary, a Distance derived instance with
//        another Distance derived instance.
//
//     double convert(const Distance& n)
//        Converts the value of a Distance derived instance into
//        the units of another Distance derived instance.
//
//
// Output stream operator:  >>
//       ostream& operator<<(ostream& sout, const Distance& n)
//          Sends "( <the Distance derived instance class name and value> )"
//          to the output stream.
//
//
// Public methods (For classes:  Meters, CentiMeters, KiloMeters,
//                               Inches, Feet, NauticalMiles,
//                               StatuteMiles):
//
// double convertStatic(const Distance& n) const
//       Static function to convert the given Distance derived
//       instance into the units of a specific Distance derived
//       class.
//
//------------------------------------------------------------------------------
class Distance : public Unit
{
    DECLARE_SUBCLASS(Distance, Unit)

public:
    Distance();
    Distance(const double);

    void set(const double v)                      { setValue(v); }
    void set(const Distance& n)                   { setValue(fromDistance(n.toDistance())); }

    double convert(const Distance& n)             { return fromDistance(n.toDistance()); }

    virtual double toDistance() const = 0;
    virtual double fromDistance(const double) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Distance& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
