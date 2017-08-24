
#include "mixr/base/ubf/AbstractAction.hpp"
#include <iostream>

namespace mixr {
namespace base {
namespace ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractAction, "AbstractAction")
EMPTY_SLOTTABLE(AbstractAction)
EMPTY_DELETEDATA(AbstractAction)

AbstractAction::AbstractAction()
{
   STANDARD_CONSTRUCTOR()
}

void AbstractAction::copyData(const AbstractAction& org, const bool cc)
{
   BaseClass::copyData(org);
   vote = org.vote;
}

}
}
}

