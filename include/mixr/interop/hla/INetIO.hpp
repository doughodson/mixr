
#ifndef __mixr_interop_hla_INetIO_H__
#define __mixr_interop_hla_INetIO_H__

#include "mixr/interop/INetIO.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/safe_ptr.hpp"

#include <array>

#include <RTI.hh>
#include <fedtime.hh>

namespace mixr {
namespace base { class Boolean; class Number; class String; }
namespace hla {
class Ambassador;
class INib;

//------------------------------------------------------------------------------
// Class: hla::INetIO
// Description: Interface code for High-Level Architecture (HLA) protocol manager.
//              Unique Federations are managed by derived classes.
//
// Factory Name: HlaNetIO
// Slots:
//      fedFile:            <base::String>    ! FED file name
//      regulatingTime:     <base::Boolean>   ! Regulating time flag
//      constrainedTime:    <base::Boolean>   ! constrained time flag
//
//------------------------------------------------------------------------------
class INetIO : public interop::INetIO
{
   DECLARE_SUBCLASS(INetIO, interop::INetIO)

public:

   INetIO();

   // Max active HLA object classes
   // Note: the class handle indexes range from 1 to MAX_CLASSES
   static const int MAX_CLASSES{30};

   // Max active HLA object attributes
   // Note: the class handle indexes range from 1 to MAX_ATTRIBUTES
   static const int MAX_ATTRIBUTES{100};

   // Max active HLA interactions
   // Note: the class handle indexes range from 1 to MAX_INTERACTIONS
   static const int MAX_INTERACTIONS{30};

   // Max active HLA interaction parameters
   // Note: the class handle indexes range from 1 to MAX_CLASSES
   static const int MAX_PARAMETERS{100};

public:
    // HLA Object Class handles
    //    Note: index range [ 1 ... MAX_CLASSES ]
    unsigned int findObjectClassIndex(const RTI::ObjectClassHandle handle) const;
    virtual unsigned int getNumberOfObjectClasses() const=0;
    virtual bool setObjectClassHandle(const unsigned int idx, const RTI::ObjectClassHandle handle);
    RTI::ObjectClassHandle getObjectClassHandle(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassHandles[idx-1] : 0; }

    // Object class published?
    bool isObjectClassPublished(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassPublished[idx-1] : 0; }
    virtual bool setObjectClassPublished(const unsigned int idx, const bool flag);

    // Object class subscribed?
    bool isObjectClassSubscribed(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassSubscribed[idx-1] : 0; }
    virtual bool setObjectClassSubscribed(const unsigned int idx, const bool flag);

    // HLA Object attribute handles
    //    Note: index range [ 1 ... MAX_ATTRIBUTES ]
    unsigned int findAttributeIndex(const RTI::AttributeHandle handle) const;
    virtual unsigned int getNumberOfObjectAttributes() const=0;
    virtual bool setObjectAttributeHandle(const unsigned int idx, const RTI::AttributeHandle handle);
    RTI::AttributeHandle getObjectAttributeHandle(const unsigned int idx) const
            { return (idx >= 1 && idx <= MAX_ATTRIBUTES) ? objectAttributeHandles[idx-1] : 0; }

    // HLA Interaction Class handles
    //    Note: index range [ 1 ... MAX_INTERACTIONS ]
    unsigned int findInteractionClassIndex(const RTI::InteractionClassHandle handle) const;
    virtual unsigned int getNumberOfOInteractionClasses() const=0;
    virtual bool setInteractionClassHandle(const unsigned int idx, const RTI::InteractionClassHandle handle);
    RTI::InteractionClassHandle getInteractionClassHandle(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassHandles[idx-1] : 0; }

    // Interaction class published?
    bool isInteractionClassPublished(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassPublished[idx-1] : 0; }
    virtual bool setInteractionClassPublished(const unsigned int idx, const bool flag);

    // Interaction class subscribed?
    bool isInteractionClassSubscribed(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassSubscribed[idx-1] : 0; }
    virtual bool setInteractionClassSubscribed(const unsigned int idx, const bool flag);

    // HLA Interaction parameter handles
    //    Note: index range [ 1 ... MAX_PARAMETERS ]
    unsigned int findParameterIndex(const RTI::ParameterHandle handle) const;
    virtual unsigned int getNumberOfInteractionParameters() const=0;
    virtual bool setInteractionParameterHandle(const unsigned int idx, const RTI::ParameterHandle handle);
    RTI::ParameterHandle getInteractionParameterHandle(const unsigned int idx) const
            { return (idx >= 1 && idx <= MAX_PARAMETERS) ? interactionParameterHandles[idx-1] : 0; }

    // Object class registration enabled flags
    virtual bool setObjectClassRegistrationFlag(const unsigned int idx, const bool flg);
    bool isObjectClassRegistrationEnabled(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? regEnbl[idx-1] : false; }

    // Interaction enabled flags
    virtual bool setInteractionEnabledFlag(const unsigned int idx, const bool flg);
    bool isInteractionEnabled(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactEnbl[idx-1] : false; }

    // Discover new objects
    virtual void discoverObjectInstance(
        const RTI::ObjectHandle theObject, const RTI::ObjectClassHandle theObjectClass, const char* theObjectName
    )=0;

    // Receive Interactions
    virtual void receiveInteraction(
        const RTI::InteractionClassHandle theInteraction,
        const RTI::ParameterHandleValuePairSet& theParameters
    )=0;


    // FED file name
    virtual const char* getFedFileName() const;

    // NIB lookup/search support
    INib* findNibByObjectHandle(RTI::ObjectHandle handle, const IoType ioType);
    INib* findNibByObjectName(const char* name, const IoType ioType);
    virtual void addNibToObjectTables(interop::INib* const nib, const IoType ioType);
    virtual void removeNibFromObjectTables(interop::INib* const nib, const IoType ioType);

    // ---
    // Other RTI functions
    //

    RTI::RTIambassador* getRTIambassador() { return rtiAmb; }  // Returns the RTI ambassador

    // Send object class attribute updates to the RTI ambassador
    virtual bool updateAttributeValues(const RTI::ObjectHandle handle, RTI::AttributeHandleValuePairSet* attrs, const char* theTag = nullptr);

    // Send an interaction to the RTI ambassador
    virtual bool sendInteraction(const RTI::InteractionClassHandle handle, RTI::ParameterHandleValuePairSet* pParams, const char* theTag = nullptr);

    RTIfedTime getLookAhead() const                         { return lookAhead; }
    void setLookAhead(const RTIfedTime time)                { lookAhead = time; }

    RTIfedTime getTimeIncrement() const                     { return timeIncrement; }
    void setTimeIncrement(const RTIfedTime time)            { timeIncrement = time; }

    RTIfedTime getFederationTime() const                    { return federationTime; }
    void setFederationTime(const RTIfedTime time)           { federationTime = time; }

    const RTI::Boolean getTimeAdvanceOutstanding() const    { return otaFlag; }
    void setTimeAdvanceOutstanding(const RTI::Boolean flag) { otaFlag = flag; }

    const RTI::Boolean getRegulating() const                { return rFlag; }
    void setRegulating(const RTI::Boolean flag)             { rFlag = flag; }

    const RTI::Boolean getConstrained() const               { return cFlag; }
    void setConstrained(const RTI::Boolean flag)            { cFlag = flag; }

    // interop::INetIO Interface
    void destroyInputNib(interop::INib* const) override;
    void destroyOutputNib(interop::INib* const) override;

protected:
    virtual void clearAllObjectClassHandles();
    virtual void clearAllObjectAttributeHandles();
    virtual void clearAllInteractionClassHandles();
    virtual void clearAllInteractionParameterHandles();
    virtual void clearAllObjectClassRegistrationFlags();
    virtual void clearAllInteractionEnabledFlags();

    // Callbacks
    virtual bool publishAndSubscribe()=0;
    virtual bool unPublishAndSubscribe();
    virtual bool unregisterAllObjects();
    virtual Ambassador* createFederateAmbassador();

    // Simulation::INetIO Interface (Callbacks)
    bool initNetwork() override;             // Initialize the network
    void netInputHander() override;          // Network input handler

    // Federation Support
    virtual bool createAndJoinFederation();
    virtual bool resignAndDestroyFederation();
    virtual bool initializeTimeManagement();
    virtual bool advanceTime();
    virtual bool doTick();

private:
   RTI::ObjectClassHandle  objectClassHandles[MAX_CLASSES];                // Object class handles
   RTI::AttributeHandle objectAttributeHandles[MAX_ATTRIBUTES];            // Object attribute handles
   std::array<bool, MAX_CLASSES> regEnbl{};                                // Object registration enabled flags
   std::array<bool, MAX_CLASSES> objectClassPublished{};                   // Object class is published
   std::array<bool, MAX_CLASSES> objectClassSubscribed{};                  // Object class is subscribed

   std::array<RTI::InteractionClassHandle, MAX_INTERACTIONS> interactionClassHandles;  // Interaction class handles
   std::array<RTI::ParameterHandle, MAX_PARAMETERS> interactionParameterHandles;       // Interaction parameter handles
   std::array<bool, MAX_INTERACTIONS> interactEnbl{};                      // Interaction enable flags
   std::array<bool, MAX_INTERACTIONS> interactionClassPublished{};         // Interaction class is published
   std::array<bool, MAX_INTERACTIONS> interactionClassSubscribed{};        // Interaction class is subscribed

   RTI::RTIambassador* rtiAmb{};                 // RTI's Ambassador
   Ambassador* fedAmb{};                         // Our Fed Ambassador
   base::safe_ptr<base::String> fedFileName;     // FED filename

   RTIfedTime lookAhead{};                       // lookahead time
   RTIfedTime timeIncrement{};                   // time to increment clock by
   RTIfedTime federationTime{};                  // current federation time
   RTI::Boolean otaFlag{RTI::RTI_FALSE};         // Outstanding time advance flag
   RTI::Boolean rFlag{RTI::RTI_FALSE};           // Time Regulating flag
   RTI::Boolean cFlag{RTI::RTI_FALSE};           // Time constrained flag

   // ---
   // Quick lookup tables
   // ---
   // input tables
   std::array<INib*, MAX_OBJECTS> inNameTbl{};    // Table of input objects in name order
   std::array<INib*, MAX_OBJECTS> inHandleTbl{};  // Table of input objects in handle order
   unsigned int nInObjects{};                    // Number of input objects in both tables

   // output tables
   std::array<INib*, MAX_OBJECTS> outNameTbl{};    // Table of output objects in name order
   std::array<INib*, MAX_OBJECTS> outHandleTbl{};  // Table of output objects in handle order
   unsigned int nOutObjects{};                    // Number of output objects in both tables

   // Support functions
   void addNibToNameTable(INib* const nib, INib** tbl, const unsigned int n);
   void addNibToHandleTable(INib* const nib, INib** tbl, const unsigned int n);
   void removeNibFromTable(INib* const nib, INib** tbl, const unsigned int n);

   // bsearch callbacks: object name compare function --
   //   True types are (const char* key, const INib** nib)
   static int compareObjNames(const void* key, const void* nib);

   // bsearch callbacks: object handle compare function --
   //   True types are (const RTI::ObjectClassHandle* key, const INib** nib)
   static int compareObjHandles(const void* key, const void* nib);

private:
    // slot table helper methods
    bool setSlotFedFile(base::String* const);
    bool setSlotRegulatingTime(base::Boolean* const);
    bool setSlotConstrainedTime(base::Boolean* const);
};

}
}

#endif


