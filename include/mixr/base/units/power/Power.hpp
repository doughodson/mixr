
#ifndef __mixr_base_Power_H__
#define __mixr_base_Power_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Power
// Description:  Base class for powers.  Defined as a Watt which is
//               equivalent to an instance of Watts with its value equal
//               to 1.0.
//
//               Base unit for Power derivations are Watts.
//
// Public methods (Defined in Power, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Power derived instance with an double.
//
//     set(const Power& n)
//        Sets, and converts if necessary, a Power derived instance with
//        another Power derived instance.
//
//     double convert(const Power& n)
//        Converts the value of a Power derived instance into
//        the units of another Power derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Power& n)
//        Sends "( <the Power derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  KiloWatts, Watts, Horsepower, DecibelWatts,
//                 DecibelMilliWatts):
//
//     double convertStatic(const Power& n)
//        Static function to convert the given Power derived
//        instance into the units of another Power derived class.
//
//----------------------------------------------------------------------------
class Power : public Unit
{
    DECLARE_SUBCLASS(Power, Unit)

public:
    Power();
    Power(const double);

    void set(const double v)           { setValue(v); }
    void set(const Power& n)           { setValue(fromPower(n.toPower())); }

    double convert(const Power& n)     { return fromPower(n.toPower()); }

    virtual double toPower() const =0;
    virtual double fromPower(const double a) const =0;
};

inline std::ostream& operator<<(std::ostream& sout, const Power& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
