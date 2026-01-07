
#ifndef __mixr_interop_hla_Ambassador_H__
#define __mixr_interop_hla_Ambassador_H__

#include <RTI.hh>
#include <fedtime.hh>

namespace mixr {
namespace hla {
class INetIO;

//------------------------------------------------------------------------------
// Class: Ambassador
// Base class: RTI::FederateAmbassador -> hla::Ambassador
//
// Description: Basic ambassador to the Federation.  Our federate unique
//              ambassadors will be derived from this class.  (Notes on
//              parameter passing are in <federateAmbServices.hh>, which
//              is included in <RTI.hh>)
//
// Public member functions:
//
//      hla::INetIO* getNetIO()
//          Returns a pointer to the HLA Network I/O class that is controlling
//          this ambassador.
//
//------------------------------------------------------------------------------
class Ambassador : public RTI::FederateAmbassador
{
public:
   Ambassador(INetIO* nio);
   virtual ~Ambassador();

   INetIO* getNetIO()                           { return hlaIo; }

private:
   INetIO* hlaIo {};         // Our controlling NetIO

// ---
// -- Standard FederateAmbassador interface --
// ---
public:
   // 4.7
   virtual void synchronizationPointRegistrationSucceeded (
     const char* label); // supplied C4

   virtual void synchronizationPointRegistrationFailed (
     const char* label); // supplied C4

   // 4.8
   virtual void announceSynchronizationPoint (
     const char* label,  // supplied C4
     const char* tag);   // supplied C4

   // 4.10
   virtual void federationSynchronized (
     const char* label);  // supplied C4)

   // 4.12
   virtual void initiateFederateSave (
     const char* label);  // supplied C4

   // 4.15
   virtual void federationSaved ();

   virtual void federationNotSaved ();

   // 4.17
   virtual void requestFederationRestoreSucceeded (
     const char* label);  // supplied C4

   virtual void requestFederationRestoreFailed (
     const char* label,   // supplied C4
     const char* reason); // supplied C4

   // 4.18
   virtual void federationRestoreBegun ();

   // 4.19
   virtual void initiateFederateRestore (
     const char* label,   // supplied C4
           RTI::FederateHandle handle);  // supplied C1

   // 4.21
   virtual void federationRestored ();

   virtual void federationNotRestored ();

   /////////////////////////////////////
   // Declaration Management Services //
   /////////////////////////////////////

   // 5.10
   virtual void startRegistrationForObjectClass (
           RTI::ObjectClassHandle   theClass);      // supplied C1

   // 5.11
   virtual void stopRegistrationForObjectClass (
           RTI::ObjectClassHandle   theClass);      // supplied C1

   // 5.12
   virtual void turnInteractionsOn (
     RTI::InteractionClassHandle theHandle); // supplied C1

   // 5.13
   virtual void turnInteractionsOff (
     RTI::InteractionClassHandle theHandle);// supplied C1

   ////////////////////////////////
   // Object Management Services //
   ////////////////////////////////

   // 6.3
   virtual void discoverObjectInstance (
           RTI::ObjectHandle          theObject,       // supplied C1
           RTI::ObjectClassHandle     theObjectClass,  // supplied C1
     const char*                      theObjectName);  // supplied C4

   // 6.5
   virtual void reflectAttributeValues (
           RTI::ObjectHandle                 theObject,     // supplied C1
     const RTI::AttributeHandleValuePairSet& theAttributes, // supplied C4
     const RTI::FedTime&                     theTime,       // supplied C1
     const char*                             theTag,        // supplied C4
           RTI::EventRetractionHandle        theHandle);    // supplied C1

   virtual void reflectAttributeValues (
           RTI::ObjectHandle                 theObject,     // supplied C1
     const RTI::AttributeHandleValuePairSet& theAttributes, // supplied C4
     const char*                             theTag);       // supplied C4

   // 6.7
   virtual void receiveInteraction (
           RTI::InteractionClassHandle       theInteraction, // supplied C1
     const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
     const RTI::FedTime&                     theTime,        // supplied C4
     const char*                             theTag,         // supplied C4
           RTI::EventRetractionHandle        theHandle);     // supplied C1

   virtual void receiveInteraction (
           RTI::InteractionClassHandle       theInteraction, // supplied C1
     const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
     const char*                             theTag);        // supplied C4

   // 6.9
   virtual void removeObjectInstance (
           RTI::ObjectHandle          theObject,  // supplied C1
     const RTI::FedTime&              theTime,    // supplied C4
     const char*                      theTag,     // supplied C4
           RTI::EventRetractionHandle theHandle); // supplied C1

   virtual void removeObjectInstance (
           RTI::ObjectHandle          theObject, // supplied C1
     const char*                      theTag);   // supplied C4

   // 6.13
   virtual void attributesInScope (
           RTI::ObjectHandle        theObject,      // supplied C1
     const RTI::AttributeHandleSet& theAttributes); // supplied C4

   // 6.14
   virtual void attributesOutOfScope (
           RTI::ObjectHandle        theObject,      // supplied C1
     const RTI::AttributeHandleSet& theAttributes); // supplied C4

   // 6.16
   virtual void provideAttributeValueUpdate (
           RTI::ObjectHandle        theObject,      // supplied C1
     const RTI::AttributeHandleSet& theAttributes); // supplied C4

   // 6.17
   virtual void turnUpdatesOnForObjectInstance (
           RTI::ObjectHandle        theObject,      // supplied C1
     const RTI::AttributeHandleSet& theAttributes); // supplied C4

   // 6.18
   virtual void turnUpdatesOffForObjectInstance (
           RTI::ObjectHandle        theObject,      // supplied C1
     const RTI::AttributeHandleSet& theAttributes); // supplied C4

   ///////////////////////////////////
   // Ownership Management Services //
   ///////////////////////////////////

   // 7.4
   virtual void requestAttributeOwnershipAssumption (
           RTI::ObjectHandle        theObject,         // supplied C1
     const RTI::AttributeHandleSet& offeredAttributes, // supplied C4
     const char*                    theTag);           // supplied C4

   // 7.5
   virtual void attributeOwnershipDivestitureNotification (
           RTI::ObjectHandle        theObject,           // supplied C1
     const RTI::AttributeHandleSet& releasedAttributes); // supplied C4

   // 7.6
   virtual void attributeOwnershipAcquisitionNotification (
           RTI::ObjectHandle        theObject,          // supplied C1
     const RTI::AttributeHandleSet& securedAttributes); // supplied C4

   // 7.9
   virtual void attributeOwnershipUnavailable (
           RTI::ObjectHandle        theObject,         // supplied C1
     const RTI::AttributeHandleSet& theAttributes);    // supplied C4

   // 7.10
   virtual void requestAttributeOwnershipRelease (
           RTI::ObjectHandle        theObject,           // supplied C1
     const RTI::AttributeHandleSet& candidateAttributes, // supplied C4
     const char*                    theTag);             // supplied C4

   // 7.14
   virtual void confirmAttributeOwnershipAcquisitionCancellation (
           RTI::ObjectHandle        theObject,         // supplied C1
     const RTI::AttributeHandleSet& theAttributes);    // supplied C4

   // 7.16
   virtual void informAttributeOwnership (
     RTI::ObjectHandle    theObject,    // supplied C1
     RTI::AttributeHandle theAttribute, // supplied C1
     RTI::FederateHandle  theOwner);    // supplied C1

   virtual void attributeIsNotOwned (
     RTI::ObjectHandle    theObject,     // supplied C1
     RTI::AttributeHandle theAttribute); // supplied C1

   virtual void attributeOwnedByRTI (
     RTI::ObjectHandle    theObject,     // supplied C1
     RTI::AttributeHandle theAttribute); // supplied C1

   //////////////////////////////
   // Time Management Services //
   //////////////////////////////

   // 8.3
   virtual void timeRegulationEnabled (
    const  RTI::FedTime& theFederateTime); // supplied C4

   // 8.6
   virtual void timeConstrainedEnabled (
     const RTI::FedTime& theFederateTime); // supplied C4

   // 8.13
   virtual void timeAdvanceGrant (
     const RTI::FedTime& theTime); // supplied C4

   // 8.22
   virtual void requestRetraction (
     RTI::EventRetractionHandle theHandle); // supplied C1

};

}
}

#endif

