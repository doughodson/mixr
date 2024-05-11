
#include "mixr/interop/rprfom/NetIO.hpp"

#include "mixr/interop/common/NtmInputNode.hpp"

#include "mixr/interop/rprfom/Nib.hpp"
#include "mixr/interop/rprfom/Ntm.hpp"
#include "NtmInputNode.hpp"
#include "mixr/interop/rprfom/RprFom.hpp"
#include "mixr/interop/hla/Ambassador.hpp"

#include "mixr/models/common/player/air/AirVehicle.hpp"
#include "mixr/models/common/player/ground/GroundVehicle.hpp"
#include "mixr/models/common/player/weapon/Missile.hpp"
#include "mixr/models/common/player/LifeForm.hpp"
#include "mixr/models/common/player/Player.hpp"
#include "mixr/models/common/player/Ship.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/network/NetHandler.hpp"
#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace rprfom {

IMPLEMENT_SUBCLASS(NetIO, "RprFomNetIO")
EMPTY_SLOTTABLE(NetIO)
EMPTY_DELETEDATA(NetIO)

NetIO::NetIO()
{
   STANDARD_CONSTRUCTOR()
}

void NetIO::copyData(const NetIO& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// getNumberOfObjectClasses() -- Number of object classes we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfObjectClasses() const
{
    return NUM_OBJECT_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfObjectAttributes() -- Number of object attributes we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfObjectAttributes() const
{
    return NUM_OBJECT_ATTRIBUTES;
}

//------------------------------------------------------------------------------
// getNumberOfOInteractionClasses() -- Number of interaction classes we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfOInteractionClasses() const
{
    return NUM_INTERACTION_CLASSES;
}

//------------------------------------------------------------------------------
// getNumberOfInteractionParameters() -- Number of interaction parameters we're using
//------------------------------------------------------------------------------
unsigned int NetIO::getNumberOfInteractionParameters() const
{
    return NUM_INTERACTION_PARAMETER;
}

//------------------------------------------------------------------------------
// Create a new output NIB
//------------------------------------------------------------------------------
interop::Nib* NetIO::createNewOutputNib(models::Player* const player)
{
    // ---
    // Check if we are enabled to register this class of objects and
    // create the proper FOM class structure
    // ---
    unsigned int idx {};
    BaseEntity* baseEntity {};
    if (player->isClassType(typeid(models::AirVehicle))) {
        if (isObjectClassRegistrationEnabled( AIRCRAFT_CLASS )) {
            baseEntity = new Aircraft();
            idx = AIRCRAFT_CLASS;
        }
    } else if (player->isClassType(typeid(models::Missile))) {
        if (isObjectClassRegistrationEnabled( MUNITION_CLASS )) {
            baseEntity = new Munition();
            idx = MUNITION_CLASS;
        }
    } else if (player->isClassType(typeid(models::LifeForm))) {
        if (isObjectClassRegistrationEnabled( HUMAN_CLASS )) {
            baseEntity = new Human();
            idx = HUMAN_CLASS;
        }
    } else if (player->isClassType(typeid(models::GroundVehicle))) {
        if (isObjectClassRegistrationEnabled( GROUND_VEHICLE_CLASS )) {
            baseEntity = new GroundVehicle();
            idx = GROUND_VEHICLE_CLASS;
        }
    } else if (player->isClassType(typeid(models::Ship))) {
        if (isObjectClassRegistrationEnabled( SURFACE_VESSEL_CLASS )) {
            baseEntity = new SurfaceVessel();
            idx = SURFACE_VESSEL_CLASS;
        }
    }

    // ---
    // If enabled, create and set the output NIB
    // ---
    Nib* nib {};
    if (baseEntity != nullptr) {
        nib = static_cast<Nib*>(nibFactory(interop::NetIO::OUTPUT_NIB));
        if (nib != nullptr) {
           nib->setBaseEntity(baseEntity);
           nib->setNetIO(this);
           nib->setPlayer(player);
           nib->setClassIndex(idx);
           nib->setOutputPlayerType(player);
        }
        baseEntity->unref();  // the NIB should have it
    }
    return nib;
}


//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool NetIO::publishAndSubscribe()
{
   bool ok {true};

   // ----------
   // We want the callbacks for attribute level advisories
   // ----------
   try {
      std::cout << "*** Enable Attribute Advisories" << std::endl;
      getRTIambassador()->enableAttributeRelevanceAdvisorySwitch();
   }
   catch (RTI::Exception& e) {
      std::cerr << &e << std::endl;
      ok = false;
   }

   // ----------
   // Get handles to the class, attributes and parameters
   // and publish & Subscribe to class attributes
   // ----------

   if (ok) ok = publishAndSubscribePlatforms();
   if (ok) ok = publishAndSubscribeWeaponFire();
   if (ok) ok = publishAndSubscribeMunitionDetonation();

   return ok;
}

//------------------------------------------------------------------------------
// processInputList() -- Update players/systems from the Input-list
//------------------------------------------------------------------------------
void NetIO::processInputList()
{
   for (unsigned int idx = 0; idx < getInputListSize(); idx++) {
      Nib* nib {static_cast<Nib*>( getInputNib(idx) )};
      if (nib != nullptr) nib->updateTheIPlayer();
   }
}

//------------------------------------------------------------------------------
// nibFactory() -- Create a new NIB
//------------------------------------------------------------------------------
interop::Nib* NetIO::nibFactory(const interop::NetIO::IoType ioType)
{
    return new Nib(ioType);
}

//------------------------------------------------------------------------------
// discoverObjectInstance() -- handle the discover of an object
//------------------------------------------------------------------------------
void NetIO::discoverObjectInstance(
        const RTI::ObjectHandle theObject,
        const RTI::ObjectClassHandle theObjectClass,
        const char* theObjectName)
{
   BaseEntity* baseEntity {};

   unsigned int idx {findObjectClassIndex(theObjectClass)};
   switch (idx) {
      case AIRCRAFT_CLASS :
         baseEntity = new Aircraft();
         break;
      case MUNITION_CLASS :
         baseEntity = new Munition();
         break;
      case HUMAN_CLASS :
         baseEntity = new Human();
         break;
      case GROUND_VEHICLE_CLASS :
         baseEntity = new GroundVehicle();
         break;
      case SURFACE_VESSEL_CLASS :
         baseEntity = new SurfaceVessel();
         break;
   };

   if (baseEntity != nullptr) {
      const auto nib = dynamic_cast<Nib*>( createNewInputNib() );
      if (nib != nullptr) {
         nib->setObjectHandle(theObject);
         nib->setObjectName(theObjectName);
         nib->setClassIndex(idx);
         nib->setTimeExec( static_cast<double>(getCurrentTime()) );
         nib->setBaseEntity(baseEntity);
         addNib2InputList(nib);
         addNibToObjectTables(nib, INPUT_NIB);
         nib->unref();
      }
      baseEntity->unref();  // (NIB has it now)
   }
}

//------------------------------------------------------------------------------
// receiveInteraction() -- handle the discover of an object
//------------------------------------------------------------------------------
void NetIO::receiveInteraction(
    const RTI::InteractionClassHandle theInteraction,
    const RTI::ParameterHandleValuePairSet& theParameters)
{
    // Select the proper method to handle this interaction
    switch( findInteractionClassIndex(theInteraction) ) {

        case WEAPON_FIRE_INTERACTION :
            receiveWeaponFire(theParameters);
            break;

        case MUNITION_DETONATION_INTERACTION :
            receiveMunitionDetonation(theParameters);
            break;
    };
}

//------------------------------------------------------------------------------
// Finds the Ntm by the entity type codes
//------------------------------------------------------------------------------
const Ntm* NetIO::findNtmByTypeCodes(
         const unsigned char  kind,
         const unsigned char  domain,
         const unsigned short countryCode,
         const unsigned char  category,
         const unsigned char  subcategory,
         const unsigned char  specific,
         const unsigned char  extra
      ) const
{
   const rprfom::Ntm* result {};

   const auto root = dynamic_cast<const rprfom::NtmInputNode*>( getRootNtmInputNode() );
   if (root != nullptr) {
      result = root->findNtmByTypeCodes(kind, domain, countryCode, category, subcategory, specific, extra);
   }
   return result;
}

//------------------------------------------------------------------------------
// root incoming NTM node factory
//------------------------------------------------------------------------------
interop::NtmInputNode* NetIO::rootNtmInputNodeFactory() const
{
   return new rprfom::NtmInputNode(rprfom::NtmInputNode::ROOT_LVL, 0); // root level
}

}
}
