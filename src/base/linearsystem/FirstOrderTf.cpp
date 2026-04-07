
#include "mixr/base/linearsystem/FirstOrderTf.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(FirstOrderTf, "FirstOrderTf")
EMPTY_SLOTTABLE(FirstOrderTf)
EMPTY_DELETEDATA(FirstOrderTf)

FirstOrderTf::FirstOrderTf()
{
   STANDARD_CONSTRUCTOR()
}

}
}
