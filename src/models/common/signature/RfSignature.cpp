
#include "mixr/models/common/signature/RfSignature.hpp"

namespace mixr {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(RfSignature, "Signature")
EMPTY_SLOTTABLE(RfSignature)
EMPTY_DELETEDATA(RfSignature)
EMPTY_COPYDATA(RfSignature)

RfSignature::RfSignature()
{
   STANDARD_CONSTRUCTOR()
}

}
}
