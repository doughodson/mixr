
#include "mixr/base/units/force/PoundForces.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(PoundForces, "PoundForces")
EMPTY_SLOTTABLE(PoundForces)
EMPTY_COPYDATA(PoundForces)
EMPTY_DELETEDATA(PoundForces)

PoundForces::PoundForces() : Force()
{
    STANDARD_CONSTRUCTOR()
}

PoundForces::PoundForces(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

PoundForces::PoundForces(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}

