
#include "mixr/models/signature/IRfSignature.hpp"

namespace mixr {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(IRfSignature, "IRfSignature")
EMPTY_SLOTTABLE(IRfSignature)
EMPTY_DELETEDATA(IRfSignature)
EMPTY_COPYDATA(IRfSignature)

IRfSignature::IRfSignature()
{
   STANDARD_CONSTRUCTOR()
}

}
}
