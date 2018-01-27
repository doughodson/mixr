
#ifndef __mixr_base_Mass_H__
#define __mixr_base_Mass_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Mass
// Description:  Base class for Masses.  Defined as a KiloGram which is
//               equivalent to an instance of KiloGrams with its value equal
//               to 1.0.
//
// Public methods (Defined in Mass, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Mass derived instance with an double.
//
//     set(const Mass& n)
//        Sets, and converts if necessary, an Mass derived instance with
//        another Mass derived instance.
//
//     double convert(const Mass& n)
//        Converts the value of an Mass derived instance into
//        the units of another Mass derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Mass& n)
//        Sends "( <the Mass derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Grams, KiloGrams, Slugs):
//
//     double convertStatic(const Mass& n)
//        Static function to convert the given Mass derived instance
//        into the units of a specific Mass derived class.
//
//------------------------------------------------------------------------------
class Mass : public Unit
{
    DECLARE_SUBCLASS(Mass, Unit)

public:
    Mass();
    Mass(const double);

    void set(const double v)              { setValue(v); }
    void set(const Mass& n)               { setValue(fromMass(n.toMass())); }

    double convert(const Mass& n) const   { return fromMass(n.toMass()); }

    //this goes to another mass (kilograms)
    virtual double toMass() const = 0;
    //this is coming from another mass (kilograms)
    virtual double fromMass(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Mass& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
