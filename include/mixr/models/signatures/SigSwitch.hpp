
#ifndef __mixr_models_SigSwitch_H__
#define __mixr_models_SigSwitch_H__

#include "mixr/models/signatures/RfSignature.hpp"

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
class SigSwitch : public RfSignature
{
   DECLARE_SUBCLASS(SigSwitch, RfSignature)
public:
   SigSwitch();

   double getRCS(const Emission* const) override;
};

}
}

#endif
