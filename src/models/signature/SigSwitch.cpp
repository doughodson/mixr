
#include "mixr/models/signature/SigSwitch.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/Emission.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigSwitch, "SigSwitch")
EMPTY_SLOTTABLE(SigSwitch)
EMPTY_DELETEDATA(SigSwitch)
EMPTY_COPYDATA(SigSwitch)

SigSwitch::SigSwitch()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigSwitch::getRCS(const Emission* const em)
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
         const auto sig = dynamic_cast<RfSignature*>( pair->object() );
         if (sig != nullptr) {

            // OK -- we've found the correct RfSignature subcomponent
            // now let it do all of the work
            rcs = sig->getRCS(em);

         }
      }
   }
   return rcs;
}

}
}
