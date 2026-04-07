
#include "mixr/base/linearsystem/FirstOrderTf.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(FirstOrderTf, "FirstOrderTf")
EMPTY_SLOTTABLE(FirstOrderTf)
EMPTY_DELETEDATA(FirstOrderTf)

FirstOrderTf::FirstOrderTf()
{
   STANDARD_CONSTRUCTOR()
}

void FirstOrderTf::copyData(const FirstOrderTf& org, const bool)
{
    BaseClass::copyData(org);
}


}
}
