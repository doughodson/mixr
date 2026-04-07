
#include "mixr/base/linearsystem/SecondOrderTf.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SecondOrderTf, "SecondOrderTf")
EMPTY_SLOTTABLE(SecondOrderTf)
EMPTY_DELETEDATA(SecondOrderTf)

SecondOrderTf::SecondOrderTf()
{
   STANDARD_CONSTRUCTOR()
}

void SecondOrderTf::copyData(const SecondOrderTf& org, const bool)
{
    BaseClass::copyData(org);
}

}
}
