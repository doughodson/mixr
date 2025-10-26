
#ifndef __mixr_simulation_IIgHost_HPP__
#define __mixr_simulation_IIgHost_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace base { class IPairStream; }
namespace simulation {
class IPlayer;

//------------------------------------------------------------------------------
// Class: IIgHost
// Description: Interface to image generator visual system host
//
// Notes:
//    1) Derived versions of this class are used to interface with various
//       image generators
//
//    2) One or more IgHost objects can be attached to and managed by a Station object
//
// Factory name: AbstractIgHost
//------------------------------------------------------------------------------
class IIgHost : public base::IComponent
{
   DECLARE_SUBCLASS(IIgHost, base::IComponent)

public:
   IIgHost();

   // sets our ownship and player list pointers, used by Station class
   virtual void setOwnship(IPlayer* const) =0;
   virtual void setPlayerList(base::IPairStream* const) =0; // Sets the player list that we're to use to generate player/models

   void updateTC(const double dt = 0.0) final {
      BaseClass::updateTC(dt);
      updateIg(dt);
   }

private:
   // main work function that manages IG communications
   virtual void updateIg(const double dt = 0.0) =0;
};

}
}

#endif
