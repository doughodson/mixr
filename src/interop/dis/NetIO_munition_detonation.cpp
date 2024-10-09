//------------------------------------------------------------------------------
// Class: NetIO
// Description: Portions of class defined to support munition detonation PDUs
//------------------------------------------------------------------------------

#include "mixr/interop/dis/NetIO.hpp"
#include "mixr/interop/dis/Nib.hpp"
#include "mixr/interop/dis/pdu.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/player/weapon/AbstractWeapon.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/base/util/nav_utils.hpp"

#include "mixr/base/network/NetHandler.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace dis {

//------------------------------------------------------------------------------
// processDetonationPDU() callback --
//------------------------------------------------------------------------------
void NetIO::processDetonationPDU(const DetonationPDU* const pdu)
{
   // Get the Firing Player's ID
   unsigned short fPlayerId      {pdu->firingEntityID.ID};
   unsigned short fSiteId        {pdu->firingEntityID.simulationID.siteIdentification};
   unsigned short fApplicationId {pdu->firingEntityID.simulationID.applicationIdentification};

   // Ignore our own PDUs
   if (fSiteId == getSiteID() && fApplicationId == getApplicationID()) return;

   // Get the Munition Player's ID
   unsigned short mPlayerId      {pdu->munitionID.ID};
   unsigned short mSiteId        {pdu->munitionID.simulationID.siteIdentification};
   unsigned short mApplicationId {pdu->munitionID.simulationID.applicationIdentification};

   // Get the Target Player's ID
   unsigned short tPlayerId      {pdu->targetEntityID.ID};
   unsigned short tSiteId        {pdu->targetEntityID.simulationID.siteIdentification};
   unsigned short tApplicationId {pdu->targetEntityID.simulationID.applicationIdentification};

   // ---
   // 1) Find the target player
   // ---
   models::Player* tPlayer {};
   if (tPlayerId != 0 && tSiteId != 0 && tApplicationId != 0) {
      interop::Nib* tNib {findDisNib(tPlayerId, tSiteId, tApplicationId, OUTPUT_NIB)};
      if (tNib != nullptr) {
         tPlayer = tNib->getPlayer();
      }
   }
   //std::cout << "Net kill(2) tPlayer = " << tPlayer << std::endl;

   // ---
   // 2) Find the firing player and munitions proxy players
   // ---
   models::Player* fPlayer {};
   if (fPlayerId != 0 && fSiteId != 0 && fApplicationId != 0) {
      interop::Nib* fNib {findDisNib(fPlayerId, fSiteId, fApplicationId, INPUT_NIB)};
      if (fNib != nullptr) {
         fPlayer = fNib->getPlayer();
      } else {
         base::safe_ptr<base::PairStream> players( getSimulation()->getPlayers() );
         fPlayer = dynamic_cast<models::Player*>(getSimulation()->findPlayer(fPlayerId));   // added DDH
      }
   }

   interop::Nib* mNib {};
   if (mPlayerId != 0 && mSiteId != 0 && mApplicationId != 0) {
      mNib = findDisNib(mPlayerId, mSiteId, mApplicationId, INPUT_NIB);
   }

    //std::cout << "Net kill(3) fNib = " << fNib << ", mNib = " << mNib << std::endl;

   // ---
   // 3) Update the data of the munition's NIB and player
   // ---
   models::AbstractWeapon* mPlayer {};
   if (mNib != nullptr) {

      // ---
      // a) Set the munition's NIB to the location of the detonation
      // ---

      // Get the geocentric position, velocity and acceleration from the PDU
      base::Vec3d geocPos;
      geocPos[base::nav::IX] = pdu->location.X_coord;
      geocPos[base::nav::IY] = pdu->location.Y_coord;
      geocPos[base::nav::IZ] = pdu->location.Z_coord;

      base::Vec3d geocVel;
      geocVel[base::nav::IX] = pdu->velocity.component[0];
      geocVel[base::nav::IY] = pdu->velocity.component[1];
      geocVel[base::nav::IZ] = pdu->velocity.component[2];

      base::Vec3d geocAcc(0,0,0);
      base::Vec3d geocAngles(0,0,0);
      base::Vec3d arates(0,0,0);

      // (re)initialize the dead reckoning function
      mNib->resetDeadReckoning(
         interop::Nib::STATIC_DRM,
         geocPos,
         geocVel,
         geocAcc,
         geocAngles,
         arates);

      // Set the NIB's mode to DETONATED
      mNib->setMode(models::Player::Mode::DETONATED);

      // Find the munition player and set its mode, location and target position
      mPlayer = dynamic_cast<models::AbstractWeapon*>(mNib->getPlayer());
      if (mPlayer != nullptr) {

         // Munition's mode
         mPlayer->setMode(models::Player::Mode::DETONATED);

         // munition's position, velocity and acceleration at the time of the detonation
         mPlayer->setGeocPosition(geocPos);
         mPlayer->setGeocVelocity(geocVel);
         mPlayer->setGeocAcceleration(geocAcc);

         // detonation results
         mPlayer->setDetonationResults(models::AbstractWeapon::Detonation(pdu->detonationResult));

         // Munition's target player and the location of detonation relative to target
         mPlayer->setTargetPlayer(tPlayer,false);
         const double x {pdu->locationInEntityCoordinates.component[0]};
         const double y {pdu->locationInEntityCoordinates.component[1]};
         const double z {pdu->locationInEntityCoordinates.component[2]};
         base::Vec3d loc(x,y,z);
         mPlayer->setDetonationLocation(loc);

         // Munition's launcher
         if (mPlayer->getLaunchVehicle() == nullptr && fPlayer != nullptr) {
            mPlayer->setLaunchVehicle(fPlayer);
         }
      }
   }

   // ---
   // 4) Check all local players for the effects of the detonation
   // ---
   if (mPlayer != nullptr) {
      mPlayer->checkDetonationEffect();
   }
}

}
}
