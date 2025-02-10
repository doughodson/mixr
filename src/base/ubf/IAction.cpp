
#include "mixr/base/ubf/IAction.hpp"
#include <iostream>

namespace mixr {
namespace base {
namespace ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(IAction, "IAction")
EMPTY_SLOTTABLE(IAction)
EMPTY_DELETEDATA(IAction)

IAction::IAction()
{
   STANDARD_CONSTRUCTOR()
}

void IAction::copyData(const IAction& org, const bool cc)
{
   BaseClass::copyData(org);
   vote = org.vote;
}

}
}
}

