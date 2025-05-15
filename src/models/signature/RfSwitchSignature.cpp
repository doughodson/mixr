
#include "mixr/models/signature/RfSwitchSignature.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/Emission.hpp"

#include "mixr/base/Pair.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(RfSwitchSignature, "RfSwitchSignature")
EMPTY_SLOTTABLE(RfSwitchSignature)
EMPTY_DELETEDATA(RfSwitchSignature)
EMPTY_COPYDATA(RfSwitchSignature)

RfSwitchSignature::RfSwitchSignature()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double RfSwitchSignature::getRCS(const Emission* const em)
{
   double rcs{};

   // Find our ownship player ...
   const Player* ownship{static_cast<const Player*>(findContainerByType(typeid(Player)))};
   if (ownship != nullptr) {

      // get our ownship's camouflage type
      unsigned int camouflage{ownship->getCamouflageType()};
      camouflage++; // our components are one based

      // find a RfSignature with this index
      base::Pair* pair{findByIndex(camouflage)};
      if (pair != nullptr) {
         const auto sig = dynamic_cast<IRfSignature*>( pair->object() );
         if (sig != nullptr) {

            // OK -- we've found the correct IRfSignature subcomponent
            // now let it do all of the work
            rcs = sig->getRCS(em);

         }
      }
   }
   return rcs;
}

}
}
