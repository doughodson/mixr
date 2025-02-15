
#include "mixr/interop/hla/rprfom/NetIO.hpp"
#include "mixr/interop/hla/rprfom/RprFom.hpp"
#include "mixr/interop/hla/rprfom/Nib.hpp"

#include "mixr/interop/hla/Ambassador.hpp"

#include "mixr/models/player/weapon/IWeapon.hpp"
#include "mixr/models/player/Player.hpp"

#include "mixr/base/network/INetHandler.hpp"

#include "mixr/base/util/nav_utils.hpp"
#include "mixr/base/util/str_utils.hpp"

namespace mixr {
namespace rprfom {

//------------------------------------------------------------------------------
// Class: Nib
// Description: Portions of class defined to support weapon fire
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// weaponFireMsgFactory() -- (Output support) Weapon fire message factory
//------------------------------------------------------------------------------
bool Nib::weaponFireMsgFactory(const double)
{
   std::cout << "rprfom::Nib::sendWeaponFire() HERE!" << std::endl;

   // Early out -- we must be registered
   if (!isRegistered()) return false;

   // Get our Simulation::NetIO
   NetIO* netIO {static_cast<NetIO*>(getNetIO())};

   // Create the parameter/value set
   RTI::ParameterHandleValuePairSet* pParams {RTI::ParameterSetFactory::create( NetIO::NUM_INTERACTION_PARAMETER )};

   // Set our mode so that we don't do this again.
   setMode(models::Player::Mode::ACTIVE);

   // If our player just launched, then it must be a weapon!
   const auto mPlayer = dynamic_cast<models::IWeapon*>(getPlayer());
   if (mPlayer == nullptr) return false;  // Early out -- it wasn't a weapon!

   // ---
   // Event ID
   // ---
   unsigned short fireEvent {mPlayer->getReleaseEventID()};
   EventIdentifierStruct eventIdentifier;
   base::INetHandler::toNetOrder(&eventIdentifier.eventCount, fireEvent);
   base::utStrncpy(
      reinterpret_cast<char*>(&eventIdentifier.issuingObjectIdentifier.id[0]),
      sizeof(eventIdentifier.issuingObjectIdentifier.id),
      getObjectName(),
      RTIObjectIdStruct::ID_SIZE );
   pParams->add(
      netIO->getInteractionParameterHandle(NetIO::EVENT_IDENTIFIER_WF_PI),
      reinterpret_cast<char*>(&eventIdentifier),
      sizeof(EventIdentifierStruct) );
   setWeaponFireEvent( fireEvent );  // remember for the detonation interaction

   // ---
   // Location & Velocity
   // ---
   {
      base::Vec3d geocPos = getDrPosition();
      base::Vec3d geocVel = getDrVelocity();
      base::Vec3d geocAcc = getDrAcceleration();

      // World Coordinates
      WorldLocationStruct firingLocation;
      base::INetHandler::toNetOrder(&firingLocation.x, geocPos[base::nav::IX]);
      base::INetHandler::toNetOrder(&firingLocation.y, geocPos[base::nav::IY]);
      base::INetHandler::toNetOrder(&firingLocation.z, geocPos[base::nav::IZ]);
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FIRING_LOCATION_WF_PI),
         reinterpret_cast<char*>(&firingLocation),
         sizeof(WorldLocationStruct) );

      // Velocity
      VelocityVectorStruct initialVelocityVector;
      base::INetHandler::toNetOrder(&initialVelocityVector.xVelocity, static_cast<float>(geocVel[base::nav::IX]));
      base::INetHandler::toNetOrder(&initialVelocityVector.yVelocity, static_cast<float>(geocVel[base::nav::IY]));
      base::INetHandler::toNetOrder(&initialVelocityVector.zVelocity, static_cast<float>(geocVel[base::nav::IZ]));
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::INITIAL_VELOCITY_VECTOR_WF_PI),
         reinterpret_cast<char*>(&initialVelocityVector),
         sizeof(VelocityVectorStruct) );
   }

   // ---
   // Munition Object identifier:
   // ---
   {
      RTIObjectIdStruct munitionObjectIdentifier;
      base::utStrncpy(
         reinterpret_cast<char*>(&munitionObjectIdentifier.id[0]),
         sizeof(munitionObjectIdentifier.id),
         getObjectName(),
         RTIObjectIdStruct::ID_SIZE );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::MUNITION_OBJECT_IDENTIFIER_WF_PI),
         reinterpret_cast<char*>(&munitionObjectIdentifier),
         sizeof(RTIObjectIdStruct) );
   }

   // ---
   // Firing Object identifier:
   //
   // Get the firing player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      Nib* fNib {};
      models::Player* fPlayer {mPlayer->getLaunchVehicle()};
      if (fPlayer != nullptr) {
         if (fPlayer->isProxyPlayer()) {
            fNib = dynamic_cast<Nib*>( fPlayer->getNib() );
         } else {
            fNib = dynamic_cast<Nib*>( netIO->findNib(fPlayer, interop::NetIO::OUTPUT_NIB) );
         }
      }

      if (fNib != nullptr) {
         RTIObjectIdStruct firingObjectIdentifier;
         base::utStrncpy(
            reinterpret_cast<char*>(&firingObjectIdentifier.id[0]),
            sizeof(firingObjectIdentifier.id),
            fNib->getObjectName(),
            RTIObjectIdStruct::ID_SIZE );
         pParams->add(
            netIO->getInteractionParameterHandle(NetIO::FIRING_OBJECT_IDENTIFIER_WF_PI),
            reinterpret_cast<char*>(&firingObjectIdentifier),
            sizeof(RTIObjectIdStruct) );
      }
   }

   // ---
   // Target Object identifier:
   //
   // Get the target player and its NIB.
   //   First check to see if it's an IPlayer from an HLA network.
   //   If it's not, then check our output list.
   // ---
   {
      Nib* tNib {};
      models::Player* tPlayer {mPlayer->getTargetPlayer()};
      if (tPlayer != nullptr) {
         tNib = dynamic_cast<Nib*>( tPlayer->getNib() );
         if (tNib == nullptr) tNib = dynamic_cast<Nib*>( netIO->findNib(tPlayer, interop::NetIO::OUTPUT_NIB) );
      }

      if (tNib != nullptr) {
         RTIObjectIdStruct targetObjectIdentifier;
         base::utStrncpy(
            reinterpret_cast<char*>(&targetObjectIdentifier.id[0]),
            sizeof(targetObjectIdentifier.id),
            tNib->getObjectName(),
            RTIObjectIdStruct::ID_SIZE );
         pParams->add(
            netIO->getInteractionParameterHandle(NetIO::TARGET_OBJECT_IDENTIFIER_WF_PI),
            reinterpret_cast<char*>(&targetObjectIdentifier),
            sizeof(RTIObjectIdStruct) );
      }
   }

   // ---
   // Munition Type
   // ---
   {
      EntityTypeStruct munitionType;
      munitionType.entityKind = getEntityKind();
      munitionType.domain = getEntityDomain();
      base::INetHandler::toNetOrder(&munitionType.countryCode, getEntityCountry() );
      munitionType.category  = getEntityCategory();
      munitionType.subcategory = getEntitySubcategory();
      munitionType.specific   = getEntitySpecific();
      munitionType.extra  = getEntityExtra();
      pParams->add(
            netIO->getInteractionParameterHandle(NetIO::MUNITION_TYPE_WF_PI),
            reinterpret_cast<char*>(&munitionType),
            sizeof(EntityTypeStruct)
         );
   }

   // ---
   // Fire Control Solution Range (meters)
   // ---
   {
      float fireControlSolutionRange {};
      base::INetHandler::toNetOrder(&fireControlSolutionRange, 0 );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FIRE_CONTROL_SOLUTION_RANGE_WF_PI),
         reinterpret_cast<char*>(&fireControlSolutionRange),
         sizeof(float) );
   }

   // ---
   // Fire Mission Index
   // ---
   {
      uint32_t fireMissionIndex {};
      uint32_t netBuffer {};
      base::INetHandler::toNetOrder(&netBuffer, fireMissionIndex );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FIRE_MISSION_INDEX_WF_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned long) );
   }

   // ---
   // Fuse Type (16 bit enum)
   // ---
   {
      FuseTypeEnum16 fuseType {FuseTypeOther};
      unsigned short netBuffer {};
      base::INetHandler::toNetOrder(&netBuffer, static_cast<unsigned short>(fuseType) );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::FUSE_TYPE_WF_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Quantity fired
   // ---
   {
      unsigned short quantityFired {1};
      unsigned short netBuffer {};
      base::INetHandler::toNetOrder(&netBuffer, quantityFired );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::QUANTITY_FIRED_WF_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Rate Of Fire
   // ---
   {
      unsigned short rateOfFire {};
      unsigned short netBuffer {};
      base::INetHandler::toNetOrder(&netBuffer, rateOfFire );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::RATE_OF_FIRE_WF_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Warhead type
   // ---
   {
      WarheadTypeEnum16 warheadType {WarheadTypeOther};
      unsigned short netBuffer {};
      base::INetHandler::toNetOrder(&netBuffer, static_cast<unsigned short>(warheadType) );
      pParams->add(
         netIO->getInteractionParameterHandle(NetIO::WARHEAD_TYPE_WF_PI),
         reinterpret_cast<char*>(&netBuffer),
         sizeof(unsigned short) );
   }

   // ---
   // Send the interaction
   // ---
   bool ok {netIO->sendInteraction(
         netIO->getInteractionClassHandle(NetIO::WEAPON_FIRE_INTERACTION),
         pParams )};

   // don't need this anymore
   delete pParams;

   return ok;
}

}
}
