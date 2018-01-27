
#include "mixr/base/units/frequency/Frequency.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Frequency, "AbstractFrequency")
EMPTY_SLOTTABLE(Frequency)
EMPTY_DELETEDATA(Frequency)

Frequency::Frequency()
{
    STANDARD_CONSTRUCTOR()
}

Frequency::Frequency(const double value) : Unit(value)
{
    STANDARD_CONSTRUCTOR()
}

void Frequency::copyData(const Frequency& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromFrequency(org.toFrequency()));
}

}
}
