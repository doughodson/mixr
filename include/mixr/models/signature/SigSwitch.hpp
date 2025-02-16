
#ifndef __mixr_models_common_SigSwitch_HPP__
#define __mixr_models_common_SigSwitch_HPP__

#include "mixr/models/signature/IRfSignature.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigSwitch
// Descriptions: Switch between subcomponent signatures based on ownship camouflage type
// Factory name: SigSwitch
// Note:
//  1) First pair (1:) is camouflage type 0, the second (2:) is camouflage type 1, etc.
//------------------------------------------------------------------------------
class SigSwitch : public IRfSignature
{
   DECLARE_SUBCLASS(SigSwitch, IRfSignature)
public:
   SigSwitch();

   double getRCS(const Emission* const) override;
};

}
}

#endif
