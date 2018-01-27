
#ifndef __mixr_Energy_H__
#define __mixr_Energy_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Energy
// Description:  Base class for energy.  Defined as a Joule which is
//               equivalent to an instance of Joules with its value equal
//               to 1.0.
//
// Public methods (Defined in Energy, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Energy derived instance with an double.
//
//     set(const Energy& n)
//        Sets, and converts if necessary, an Energy derived instance with
//        another Energy derived instance.
//
//     double convert(const Energy& n)
//        Converts the value of an Energy derived instance into
//        the units of another Energy derived instance.
//
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Energy& n)
//        Sends "( <the Energy derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  KiloWattHours, BTUs, Calories, Joules, FootPounds):
//
//     double convertStatic(const Energy& n)
//        Static function to convert the given Energy derived
//        instance into the units of a specific Energy derived
//        class.
//------------------------------------------------------------------------------
class Energy : public Unit
{
    DECLARE_SUBCLASS(Energy, Unit)

public:
    Energy();
    Energy(const double);

    void set(const double v)                       { setValue(v); }
    void set(const Energy& n)                      { setValue(fromEnergy(n.toEnergy())); }

    double convert(const Energy& n)                { return fromEnergy(n.toEnergy()); }

    virtual double toEnergy() const = 0;
    virtual double fromEnergy(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Energy& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
