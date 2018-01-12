
#include "mixr/base/Identifier.hpp"

#include <string>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Identifier, "Identifier")
EMPTY_SLOTTABLE(Identifier)
EMPTY_DELETEDATA(Identifier)

Identifier::Identifier()
{
   STANDARD_CONSTRUCTOR()
}

Identifier::Identifier(const char* string)
{
   STANDARD_CONSTRUCTOR()
   setStr(string);
}

void Identifier::copyData(const Identifier& org, const bool cc)
{
   BaseClass::copyData(org);
   str = org.str;
}

}
}
