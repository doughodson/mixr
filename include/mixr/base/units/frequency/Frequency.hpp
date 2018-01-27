
#ifndef __mixr_base_Frequency_H__
#define __mixr_base_Frequency_H__

#include "mixr/base/units/Unit.hpp"

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

    void set(const double v)              { setValue(v); }
    void set(const Frequency& n)          { setValue(fromFrequency(n.toFrequency())); }

    double convert(const Frequency& n)    { return fromFrequency(n.toFrequency()); }

    virtual double toFrequency() const = 0;
    virtual double fromFrequency(const double a) const = 0;
};

inline std::ostream& operator<<(std::ostream& sout, const Frequency& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
