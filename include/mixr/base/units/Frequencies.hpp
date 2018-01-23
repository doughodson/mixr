//------------------------------------------------------------------------------
// Classes:  Frequency, Hertz
//------------------------------------------------------------------------------
#ifndef __mixr_base_Frequencies_H__
#define __mixr_base_Frequencies_H__

#include "mixr/base/units/Unit.hpp"

#include "mixr/base/units/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Frequency
// Description:  Base class for frequencies.  Defined as a hertz which is
//               equivalent to an instance of Hertz with its value equal
//               to 1.0.
//
//
// Public methods (Defined in Frequency, and inherited by all derived classes):
//
//     set(const double v)
//        Sets a Frequency derived instance with an double.
//
//     set(const Frequency& n)
//        Sets, and converts if necessary, a Frequency derived instance with
//        another Frequency derived instance.
//
//     double convert(const Frequency& n)
//        Converts the value of a Frequency derived instance into
//        the units of another Frequency derived instance.
//
//     Conversion routines:
//        Not used here, because they are all metric conversions...
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Frequency& n)
//        Sends "( <the Frequency derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes: Hertz):
//
//     double convertStatic(const Frequency& n)
//        Static function to convert the given Frequency derived
//        instance into the units of a specific Frequency derived
//        class.
//
//------------------------------------------------------------------------------
class Frequency : public Unit
{
    DECLARE_SUBCLASS(Frequency, Unit)

public:
    Frequency();
    Frequency(const double);

    void set(const double v)     { val = v; }
    void set(const Frequency& n) { val = fromFrequency(n.toFrequency()); }

    double convert(const Frequency& n) { return fromFrequency(n.toFrequency()); }

    virtual double toFrequency() const = 0;
    virtual double fromFrequency(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Frequency& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class: Hertz
// Description:  Base unit for frequency, with a hertz being an instance of Hertz
//               with its value equal to 1.0.
//------------------------------------------------------------------------------
class Hertz final: public Frequency
{
    DECLARE_SUBCLASS(Hertz, Frequency)

public:
    Hertz();
    Hertz(const double);
    Hertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency(); }

    double toFrequency() const final                     { return val; }
    double fromFrequency(const double a) const final     { return a; }
};

//------------------------------------------------------------------------------
// Class: KiloHertz
// Description:  Hertz * 0.001
//------------------------------------------------------------------------------
class KiloHertz final: public Frequency
{
    DECLARE_SUBCLASS(KiloHertz, Frequency)

public:
    KiloHertz();
    KiloHertz(const double);
    KiloHertz(const Frequency&);

private:
    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2KHz; }

    double toFrequency() const final                     { return (val * frequency::KHz2Hz); }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2KHz; }
};


//------------------------------------------------------------------------------
// Class: MegaHertz
// Description:  Hertz * 0.000001
//------------------------------------------------------------------------------
class MegaHertz final: public Frequency
{
    DECLARE_SUBCLASS(MegaHertz, Frequency)

public:
    MegaHertz();
    MegaHertz(const double);
    MegaHertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2MHz; }

    double toFrequency() const final                     { return (val * frequency::MHz2Hz); }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2MHz; }
};


//------------------------------------------------------------------------------
// Class: GigaHertz
// Description:  Hertz * 0.000000001
//------------------------------------------------------------------------------
class GigaHertz final: public Frequency
{
    DECLARE_SUBCLASS(GigaHertz, Frequency)

public:
    GigaHertz();
    GigaHertz(const double);
    GigaHertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2GHz; }

    double toFrequency() const final                     { return (val * frequency::GHz2Hz); }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2GHz; }
};


//------------------------------------------------------------------------------
// Class:  TeraHertz
// Description:  Hertz * 0.000000000001
//------------------------------------------------------------------------------
class TeraHertz final: public Frequency
{
    DECLARE_SUBCLASS(TeraHertz, Frequency)

public:
    TeraHertz();
    TeraHertz(const double);
    TeraHertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2THz; }

    double toFrequency() const final                     { return static_cast<double>(val * frequency::THz2Hz); }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2THz; }
};

}
}

#endif
