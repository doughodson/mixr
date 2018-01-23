//------------------------------------------------------------------------------
// Classes:  Energy, KiloWattHours, BTUs, Calories, FootPounds, Joules
//------------------------------------------------------------------------------

#ifndef __mixr_Energies_H__
#define __mixr_Energies_H__

#include "mixr/base/units/Unit.hpp"

#include "mixr/base/units/energy_utils.hpp"

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

    void set(const double v)                       { val = v; }
    void set(const Energy& n)                      { val = fromEnergy(n.toEnergy()); }

    double convert(const Energy& n)                { return fromEnergy(n.toEnergy()); }

    virtual double toEnergy() const = 0;
    virtual double fromEnergy(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Energy& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class: Joules
// Description: An instance of Joules with its value equal to 1.0 is one
//              base unit for energy.
//------------------------------------------------------------------------------
class Joules final: public Energy
{
    DECLARE_SUBCLASS(Joules, Energy)

public:
    Joules();
    Joules(const double);
    Joules(const Energy&);

    static double convertStatic(const Energy& n)       { return n.toEnergy(); }

    double toEnergy() const final                      { return val; }
    double fromEnergy(const double a) const final      { return a; }
};

//------------------------------------------------------------------------------
// Class: KiloWattHours
// Description: Joules * 0.000000277778
//------------------------------------------------------------------------------
class KiloWattHours final: public Energy
{
    DECLARE_SUBCLASS(KiloWattHours, Energy)

public:
    KiloWattHours();
    KiloWattHours(const double);
    KiloWattHours(const Energy&);

    static double convertStatic(const Energy& n)     { return n.toEnergy() * energy::J2KWH; }

    double toEnergy() const final                    { return (val * energy::KWH2J); }
    double fromEnergy(const double a) const final    { return a * energy::J2KWH; }
};


//------------------------------------------------------------------------------
// Class: BTUs
// Description:  Joules * 9.478 x 10 -04
//------------------------------------------------------------------------------
class BTUs final: public Energy
{
    DECLARE_SUBCLASS(BTUs, Energy)

public:
    BTUs();
    BTUs(const double);
    BTUs(const Energy&);

    static double convertStatic(const Energy& n)     { return n.toEnergy() * energy::J2BTU; }

    double toEnergy() const final                    { return (val * energy::BTU2J); }
    double fromEnergy(const double a) const final    { return a * energy::J2BTU; }
};


//------------------------------------------------------------------------------
// Class: Calories
// Description:  Joules * 0.2388888888888888889
//------------------------------------------------------------------------------
class Calories final: public Energy
{
    DECLARE_SUBCLASS(Calories, Energy)

public:
    Calories();
    Calories(const double);
    Calories(const Energy&);

    static double convertStatic(const Energy& n)       { return n.toEnergy() * energy::J2C; }

    double toEnergy() const final                      { return (val * energy::C2J); }
    double fromEnergy(const double a) const final      { return a * energy::J2C; }
};


//------------------------------------------------------------------------------
// Class: FootPounds
// Description:  Joules * 0.7376
//------------------------------------------------------------------------------
class FootPounds final: public Energy
{
    DECLARE_SUBCLASS(FootPounds, Energy)

public:
    FootPounds();
    FootPounds(const double);
    FootPounds(const Energy&);

    static double convertStatic(const Energy& n)      { return n.toEnergy() * energy::J2FP; }

    double toEnergy() const final                     { return (val * energy::FP2J); }
    double fromEnergy(const double a) const final     { return a * energy::J2FP; }
};

}
}

#endif
