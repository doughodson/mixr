
#include "mixr/models/player/weapon/Sam.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Sam, "Sam")
EMPTY_SLOTTABLE(Sam)
EMPTY_COPYDATA(Sam)
EMPTY_DELETEDATA(Sam)

int Sam::getCategory() const               { return (MISSILE | GUIDED); }
const char* Sam::getDescription() const    { return "SAM"; }
const char* Sam::getNickname() const       { return "GenericSam"; }

Sam::Sam()
{
   STANDARD_CONSTRUCTOR()

   static base::String gsam(getDescription());
   setType_old(&gsam);
   setType("SAM");
}

}
}
