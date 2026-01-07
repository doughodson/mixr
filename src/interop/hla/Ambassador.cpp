
#include "mixr/interop/hla/Ambassador.hpp"
#include "mixr/interop/hla/INetIO.hpp"
#include "mixr/interop/hla/INib.hpp"

#include "mixr/models/player/IPlayer.hpp"

#include <iostream>

namespace mixr {
namespace hla {

Ambassador::Ambassador(INetIO* netIO): hlaIo(netIO)
{}

Ambassador::~Ambassador()
{}

void Ambassador::startRegistrationForObjectClass (RTI::ObjectClassHandle theClass)
{
   //std::cout << "Ambassador::startRegistrationForObjectClass(): " ;
   INetIO* netIO {getNetIO()};
   const unsigned int classIndex {netIO->findObjectClassIndex(theClass)};
   if (classIndex != 0 && netIO->isObjectClassPublished(classIndex)) {
      // It's an object class that we publish, so we can start to
      // register our objects ...
      //std::cout << classIndex;
      netIO->setObjectClassRegistrationFlag(classIndex,true);
   }
   //std::cout << std::endl;
}

void Ambassador::stopRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
{
   //std::cout << "Ambassador::stopRegistrationForObjectClass(): ";
   INetIO* netIO {getNetIO()};
   const unsigned int classIndex {netIO->findObjectClassIndex(theClass)};
   if (classIndex != 0 ) {
      //std::cout << classIndex;
      netIO->setObjectClassRegistrationFlag(classIndex,false);
   }
   //std::cout << std::endl;
}

void Ambassador::turnUpdatesOnForObjectInstance(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
{
   //std::cout << "Ambassador:turnUpdatesOnForObjectInstance(): theObject = " << theObject;

   // Find the output object
   INetIO* netIO {getNetIO()};
   INib* nib {static_cast<INib*>(netIO->findNibByObjectHandle(theObject, INetIO::OUTPUT_NIB))};

   if (nib != nullptr) nib->turnUpdatesOn(theAttributes);
   //else std::cout << " NOT FOUND";
   //std::cout << std::endl;
}

void Ambassador::turnUpdatesOffForObjectInstance(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
{
   //std::cout << "Ambassador:turnUpdatesOffForObjectInstance(): ";

   // Find the output object
   INetIO* netIO {getNetIO()};
   INib* nib {static_cast<INib*>(netIO->findNibByObjectHandle(theObject, INetIO::OUTPUT_NIB))};

   if (nib != nullptr) nib->turnUpdatesOff(theAttributes);
   //std::cout << std::endl;
}

void Ambassador::provideAttributeValueUpdate(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttrs)
{
   //std::cout << "Ambassador:provideAttributeValueUpdate(): ";
   // Find the output object
   INetIO* netIO {getNetIO()};
   INib* nib {static_cast<INib*>( netIO->findNibByObjectHandle(theObject, INetIO::OUTPUT_NIB) )};

   if (nib != nullptr) nib->provideAttributeValueUpdate(theAttrs);
   //std::cout << std::endl;
}

void Ambassador::discoverObjectInstance(RTI::ObjectHandle theObject, RTI::ObjectClassHandle theObjectClass, const char* theObjectName)
{
   std::cout << "Ambassador::discoverObjectInstance(): " << theObjectName << ", handle = " << theObject << std::endl;
   INetIO* netIO {getNetIO()};
   const unsigned int classIndex {netIO->findObjectClassIndex(theObjectClass)};
   if (classIndex != 0 && netIO->isObjectClassSubscribed(classIndex)) {
      // It's an object class that we've subscribed to ...

      // just act as an interface to NetIO, which will handle this.
      getNetIO()->discoverObjectInstance(theObject, theObjectClass, theObjectName);
   }
}

void Ambassador::removeObjectInstance(RTI::ObjectHandle theObject, const RTI::FedTime& theTime,
                                      const char* theTag, RTI::EventRetractionHandle theHandle)
{
   removeObjectInstance(theObject, theTag);
}

void Ambassador::removeObjectInstance(RTI::ObjectHandle theObject, const char* theTag)
{
   std::cout << "Ambassador::removeObjectInstance(): remove object = " << theObject << ", theTag=" << theTag << std::endl;

   // find the input object
   INetIO* netIO {getNetIO()};
   INib* nib {netIO->findNibByObjectHandle(theObject, INetIO::INPUT_NIB)};
   if (nib != nullptr) {
      // set NIB delete request (Simulation::NetIO::cleanupInputList() should handle this)
      nib->setMode(models::IPlayer::Mode::DELETE_REQUEST);
   }
}

void Ambassador::reflectAttributeValues(RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttrs,
                                        const RTI::FedTime& theTime, const char* theTag, RTI::EventRetractionHandle theHandle)
{
   reflectAttributeValues(theObject, theAttrs, theTag);
}

void Ambassador::reflectAttributeValues(RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttrs,
                                        const char* theTag)
{
   //std::cout << "Ambassador::reflectAttributeValues(): object = " << theObject << ", theTag=" << theTag << std::endl;
   // find the input object
   INetIO* netIO {getNetIO()};
   INib* nib {static_cast<INib*>( netIO->findNibByObjectHandle(theObject, INetIO::INPUT_NIB) )};
   if (nib != nullptr) nib->reflectAttributeValues(theAttrs);
}

void Ambassador::turnInteractionsOn(RTI::InteractionClassHandle theInteraction)
{
   std::cout << "Ambassador::turnInteractionsOn(): " ;
   INetIO* netIO {getNetIO()};
   const unsigned int idx {netIO->findInteractionClassIndex(theInteraction)};
   if (idx != 0 && netIO->isInteractionClassPublished(idx)) {
      // It's an interaction that we publish, so we can start
      // send new interactions of this class ...
      std::cout << idx;
      netIO->setInteractionEnabledFlag(idx,true);
   }
   std::cout << std::endl;
}

void Ambassador::turnInteractionsOff(RTI::InteractionClassHandle theInteraction)
{
   std::cout << "Ambassador::turnInteractionsOff(): " ;
   INetIO* netIO {getNetIO()};
   const unsigned int idx {netIO->findInteractionClassIndex(theInteraction)};
   if (idx != 0) {
      std::cout << idx;
      netIO->setInteractionEnabledFlag(idx,false);
   }
   std::cout << std::endl;
}

void Ambassador::receiveInteraction(RTI::InteractionClassHandle theInteraction, const RTI::ParameterHandleValuePairSet& theParameters,
                                    const RTI::FedTime& theTime, const char* theTag, RTI::EventRetractionHandle        theHandle)
{
   receiveInteraction(theInteraction, theParameters, theTag);
}

void Ambassador::receiveInteraction(RTI::InteractionClassHandle theInteraction, const RTI::ParameterHandleValuePairSet& theParameters, const char*)
{
   std::cout << "Ambassador::receiveInteraction(): " << theInteraction << std::endl;
   INetIO* netIO {getNetIO()};
   const unsigned int idx {netIO->findInteractionClassIndex(theInteraction)};
   if (idx != 0 && netIO->isInteractionClassSubscribed(idx)) {
      // It's an interaction that we subscribe to, so ...

      // just act as an interface to NetIO, which will handle this.
      getNetIO()->receiveInteraction(theInteraction, theParameters);
   }
}

//------------------------------------------------------------------------------
// Federate Ambassador functions -- Default/null functions
//------------------------------------------------------------------------------

void Ambassador::synchronizationPointRegistrationSucceeded(const char* label)
{
   std::cout << "Unhandled synchronizationPointRegistrationSucceeded()" << std::endl;
}

void Ambassador::synchronizationPointRegistrationFailed(const char *label)
{
   std::cout << "Unhandled synchronizationPointRegistrationFailed()" << std::endl;
}

void Ambassador::announceSynchronizationPoint(const char* label, const char* tag)
{
   std::cout << "Unhandled announceSynchronizationPoint()" << std::endl;
}

void Ambassador::federationSynchronized(const char *label)
{
   std::cout << "Unhandled federationSynchronized()" << std::endl;
}

void Ambassador::initiateFederateSave(const char *label)
{
   std::cout << "Unhandled initiateFederateSave()" << std::endl;
}

void Ambassador::federationSaved()
{
   std::cout << "Unhandled federationSaved()" << std::endl;
}

void Ambassador::federationNotSaved()
{
   std::cout << "Unhandled federationNotSaved()" << std::endl;
}

void Ambassador::requestFederationRestoreSucceeded(const char *label)
{
   std::cout << "Unhandled requestFederationRestoreSucceeded()" << std::endl;
}

void Ambassador::requestFederationRestoreFailed(const char* label, const char* reason)
{
   std::cout << "Unhandled requestFederationRestoreFailed()" << std::endl;
}

void Ambassador::federationRestoreBegun()
{
   std::cout << "Unhandled federationRestoreBegun()" << std::endl;
}

void Ambassador::initiateFederateRestore(const char* label, RTI::FederateHandle handle)
{
   std::cout << "Unhandled initiateFederateRestore()" << std::endl;
}

void Ambassador::federationRestored()
{
   std::cout << "Unhandled federationRestored()" << std::endl;
}

void Ambassador::federationNotRestored()
{
   std::cout << "Unhandled federationNotRestored()" << std::endl;
}

////////////////////////////////
// Object Management Services //
////////////////////////////////

void Ambassador::attributesInScope(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
{
   std::cout << "Unhandled attributesInScope()" << std::endl;
}

void Ambassador::attributesOutOfScope(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
{
   std::cout << "Unhandled attributesOutOfScope()" << std::endl;
}

///////////////////////////////////
// Ownership Management Services //
///////////////////////////////////

void Ambassador::requestAttributeOwnershipAssumption(RTI::ObjectHandle theObject,  const RTI::AttributeHandleSet& offeredAttributes,
                                                     const char* theTag)
{
   std::cout << "Unhandled requestAttributeOwnershipAssumption()" << std::endl;
}

void Ambassador::attributeOwnershipDivestitureNotification(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& releasedAttributes)
{
   std::cout << "Unhandled attributeOwnershipDivestitureNotification()" << std::endl;
}

void Ambassador::attributeOwnershipAcquisitionNotification(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& securedAttributes)
{
   std::cout << "Unhandled attributeOwnershipAcquisitionNotification()" << std::endl;
}

void Ambassador::attributeOwnershipUnavailable(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
{
   std::cout << "Unhandled attributeOwnershipUnavailable()" << std::endl;
}

void Ambassador::requestAttributeOwnershipRelease(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& candidateAttributes,
                                                  const char* theTag)
{
   std::cout << "Unhandled requestAttributeOwnershipRelease()" << std::endl;
}

void Ambassador::confirmAttributeOwnershipAcquisitionCancellation(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
{
   std::cout << "Unhandled confirmAttributeOwnershipAcquisitionCancellation()" << std::endl;
}

void Ambassador::informAttributeOwnership(RTI::ObjectHandle theObject, RTI::AttributeHandle theAttribute, RTI::FederateHandle  theOwner)
{
   std::cout << "Unhandled informAttributeOwnership()" << std::endl;
}

void Ambassador::attributeIsNotOwned(RTI::ObjectHandle theObject, RTI::AttributeHandle theAttribute)
{
   std::cout << "Unhandled attributeIsNotOwned()" << std::endl;
}

void Ambassador::attributeOwnedByRTI(RTI::ObjectHandle theObject, RTI::AttributeHandle theAttribute)
{
   std::cout << "Unhandled attributeOwnedByRTI()" << std::endl;
}

//////////////////////////////
// Time Management Services //
//////////////////////////////

void Ambassador::timeRegulationEnabled (const RTI::FedTime& theFederateTime)
{
  std::cout << "### timeRegulationEnabled callback";
  hlaIo->setFederationTime(theFederateTime);
  std::cout << "  Federate Time is " << hlaIo->getFederationTime() << std::endl;
}

void Ambassador::timeConstrainedEnabled(const RTI::FedTime& theFederateTime)
{
  std::cout << "### timeConstrainedEnabled callback";
  hlaIo->setFederationTime(theFederateTime);
  std::cout << "  Federate Time is " << hlaIo->getFederationTime() << std::endl;
}

void Ambassador::timeAdvanceGrant(const RTI::FedTime& theTime)
{
   hlaIo->setTimeAdvanceOutstanding(RTI::RTI_FALSE);
   hlaIo->setFederationTime(theTime);
//  std::cout << "### timeAdvanceGrant callback Time: " << hlaIo->getFederationTime() << endl;
}

void Ambassador::requestRetraction(RTI::EventRetractionHandle theHandle)
{
   std::cout << "Unhandled requestRetraction()" << std::endl;
}

}
}

