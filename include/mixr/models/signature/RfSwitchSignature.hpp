
#ifndef __mixr_models_common_RfSwitchSignature_HPP__
#define __mixr_models_common_RfSwitchSignature_HPP__

#include "mixr/models/signature/IRfSignature.hpp"

namespace mixr {
namespace models {
class RfEmission;

//------------------------------------------------------------------------------
// Class: RfSwitchSignature
//   Descriptions: Concrete class to switch signatures based on ownship camouflage type
//
// Factory name: RfSwitchSignature
// Note:
//   1) First pair (1:) is camouflage type 0, the second (2:) is camouflage type 1, etc.
//------------------------------------------------------------------------------
class RfSwitchSignature final: public IRfSignature
{
   DECLARE_SUBCLASS(RfSwitchSignature, IRfSignature)
public:
   RfSwitchSignature();

   double getRCS(const RfEmission* const) override;
};

}
}

#endif
