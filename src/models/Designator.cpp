
#include "mixr/models/Designator.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Designator, "Designator")
EMPTY_SLOTTABLE(Designator)
EMPTY_DELETEDATA(Designator)

Designator::Designator() : data()
{
    STANDARD_CONSTRUCTOR()
}

void Designator::copyData(const Designator& org, const bool)
{
    BaseClass::copyData(org);
}

}
}
