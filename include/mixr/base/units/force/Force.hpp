
#ifndef __mixr_base_Force_H__
#define __mixr_base_Force_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Force
// Description:  Base class for forces.  Defined as a Newton which is
//               equivalent to an instance of Newtons with its value equal
//               to 1.0.
//
// Description:  Numbers as forces -- Newtons, KiloNewtons, Poundals,
//               PoundForces
//               Base unit for Force derivations are Newtons.
//
// Public methods (Defined in Force, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Force derived instance with an double.
//
//     set(const Force& n)
//        Sets, and converts if necessary, a Force derived instance with
//        another Force derived instance.
//
//     double convert(const Force& n)
//        Converts the value of a Force derived instance into
//        the units of another Force derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Force& n)
//        Sends "( <the Force derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Newtons, KiloNewtons, PoundForce, Poundals):
//
//     double convertStatic(const Force& n)
//        static function to convert the given Force derived
//        instance into the units of a specific Force derived class.
//------------------------------------------------------------------------------
class Force : public Unit
{
    DECLARE_SUBCLASS(Force, Unit)

public:
    Force();
    Force(const double);

    void set(const double v)              { setValue(v); }
    void set(const Force& n)              { setValue(fromForce(n.toForce())); }

    double convert(const Force& n)        { return fromForce(n.toForce()); }

    virtual double toForce() const = 0;
    virtual double fromForce(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Force& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
