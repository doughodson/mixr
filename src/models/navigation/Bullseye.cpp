
#include "mixr/models/navigation/Bullseye.hpp"
#include <iostream>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Bullseye, "Bullseye")
EMPTY_SLOTTABLE(Bullseye)
EMPTY_DELETEDATA(Bullseye)
EMPTY_COPYDATA(Bullseye)

Bullseye::Bullseye()
{
    STANDARD_CONSTRUCTOR()
}

}
}
