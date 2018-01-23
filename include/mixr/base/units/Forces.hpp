//------------------------------------------------------------------------------
// Classes:  Force, PoundForces, Newtons, Poundals, KiloNewtons
//------------------------------------------------------------------------------

#ifndef __mixr_base_Forces_H__
#define __mixr_base_Forces_H__

#include "mixr/base/units/Unit.hpp"

#include "mixr/base/units/force_utils.hpp"

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

    void set(const double v) { val = v; }
    void set(const Force& n) { val = fromForce(n.toForce()); }

    double convert(const Force& n){ return fromForce(n.toForce()); }

    virtual double toForce() const = 0;
    virtual double fromForce(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Force& n)
   { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class: Newtons
// Description:  An instance of Newtons with its value equal to 1.0 is one
//               base unit for forces.
//------------------------------------------------------------------------------
class Newtons final: public Force
{
    DECLARE_SUBCLASS(Newtons, Force)

public:
    Newtons();
    Newtons(const double);
    Newtons(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce(); }

    double toForce() const final                       { return val; }
    double fromForce(const double a) const final       { return a; }
};


//------------------------------------------------------------------------------
// Class: KiloNewtons
// Description:  Newtons * 1000.0
//------------------------------------------------------------------------------
class KiloNewtons final: public Force
{
    DECLARE_SUBCLASS(KiloNewtons, Force)

public:
    KiloNewtons();
    KiloNewtons(const double);
    KiloNewtons(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce() * force::N2KN; }

    double toForce() const final                       { return (val * force::KN2N); }
    double fromForce(const double a) const final       { return a * force::N2KN; }
};


//------------------------------------------------------------------------------
// Class: PoundForces
// Description:  Newtons * 0.224809
//------------------------------------------------------------------------------
class PoundForces final: public Force
{
    DECLARE_SUBCLASS(PoundForces, Force)

public:
    PoundForces();
    PoundForces(const double);
    PoundForces(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce() * force::N2PF; }

    double toForce() const final                       { return (val * force::PF2N); }
    double fromForce(const double a) const final       { return a * force::N2PF; }
};


//------------------------------------------------------------------------------
// Class: Poundals
// Description:  Newtons * 7.23301
//------------------------------------------------------------------------------
class Poundals final: public Force
{
    DECLARE_SUBCLASS(Poundals, Force)

public:
    Poundals();
    Poundals(const double);
    Poundals(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce() * force::N2PD; }

    double toForce() const final                       { return (val * force::PD2N); }
    double fromForce(const double a) const final       { return a * force::N2PD; }
};

}
}

#endif
