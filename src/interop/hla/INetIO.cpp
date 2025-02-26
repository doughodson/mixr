
#include "mixr/interop/hla/INetIO.hpp"
#include "mixr/interop/hla/INib.hpp"
#include "mixr/interop/hla/Ambassador.hpp"

#include "mixr/base/numeric/Boolean.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/Pair.hpp"

#include "mixr/base/util/system_utils.hpp"

#include <cstring>
#include <string>

#ifndef WIN32
#include <unistd.h>
#endif

namespace mixr {
namespace hla {

IMPLEMENT_PARTIAL_SUBCLASS(INetIO, "HlaNetIO")

BEGIN_SLOTTABLE(INetIO)
   "fedFile",          // 1) FED file name
   "regulatingTime",   // 2) Regulating time flag
   "constrainedTime",  // 3) constrained time flag
END_SLOTTABLE(INetIO)

BEGIN_SLOT_MAP(INetIO)
   ON_SLOT(1, setSlotFedFile,         base::String)
   ON_SLOT(2, setSlotRegulatingTime,  base::Boolean)
   ON_SLOT(3, setSlotConstrainedTime, base::Boolean)
END_SLOT_MAP()

INetIO::INetIO()
{
   STANDARD_CONSTRUCTOR()

   clearAllObjectClassHandles();
   clearAllObjectAttributeHandles();
   clearAllInteractionClassHandles();
   clearAllInteractionParameterHandles();
   clearAllObjectClassRegistrationFlags();
   clearAllInteractionEnabledFlags();

   setFederationName(nullptr);
   setFederateName(nullptr);

   rtiAmb = new RTI::RTIambassador();
}

INetIO::INetIO(const INetIO& org)
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

INetIO::~INetIO()
{
   STANDARD_DESTRUCTOR()
}

INetIO& INetIO::operator=(const INetIO& org)
{
   deleteData();
   copyData(org, false);
   return *this;
}

INetIO* INetIO::clone() const
{
   return nullptr;
}

void INetIO::copyData(const INetIO& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      clearAllObjectClassHandles();
      clearAllObjectAttributeHandles();
      clearAllInteractionClassHandles();
      clearAllInteractionParameterHandles();
      clearAllObjectClassRegistrationFlags();
      clearAllInteractionEnabledFlags();

      setFederationName(nullptr);
      setFederateName(nullptr);
      rtiAmb = new RTI::RTIambassador();
   }

   nInObjects = 0;      // ******
   nOutObjects = 0;     // ******

   for (unsigned int i = 0; i < MAX_CLASSES; i++) {
      objectClassHandles[i] = org.objectClassHandles[i];
      objectClassPublished[i] = org.objectClassPublished[i];
      objectClassSubscribed[i] = org.objectClassSubscribed[i];
      regEnbl[i] = org.regEnbl[i];
   }

   for (unsigned int i = 0; i < MAX_ATTRIBUTES; i++) {
      objectAttributeHandles[i] = org.objectAttributeHandles[i];
   }

   for (unsigned int i = 0; i < MAX_INTERACTIONS; i++) {
      interactionClassHandles[i] = org.interactionClassHandles[i];
      interactionClassPublished[i] = org.interactionClassPublished[i];
      interactionClassSubscribed[i] = org.interactionClassSubscribed[i];
      interactEnbl[i] = org.interactEnbl[i];
   }

   for (unsigned int i = 0; i < MAX_PARAMETERS; i++) {
      interactionParameterHandles[i] = org.interactionParameterHandles[i];
   }

   {
      const base::String* s{org.fedFileName};
      if (s != nullptr) {
         fedFileName = static_cast<base::String*>(s->clone());
      }
      else {
         fedFileName = nullptr;
      }
   }

   federationTime = org.federationTime;
   otaFlag = org.otaFlag;
   rFlag = org.rFlag;
   cFlag = org.cFlag;
}

void INetIO::deleteData()
{
   if (isNetworkInitialized()) {
      unregisterAllObjects();
      unPublishAndSubscribe();
      doTick();
      // resign from the federation execution and attempt to destroy
      resignAndDestroyFederation();
   }
   setFederationName(nullptr);
   setFederateName(nullptr);
   if (rtiAmb != nullptr)    { delete rtiAmb;  rtiAmb = nullptr; }
   fedFileName = nullptr;
}


//------------------------------------------------------------------------------
// HLA Object Class handles
//    Note: class indexes range [ 1 ... MAX_CLASSES ]
//------------------------------------------------------------------------------
bool INetIO::setObjectClassHandle(const unsigned int idx, const RTI::ObjectClassHandle handle)
{
    bool ok = (idx >= 1 && idx <= MAX_CLASSES);
    if (ok) objectClassHandles[idx-1] = handle;
    return ok;
}

bool INetIO::setObjectClassPublished(const unsigned int idx, const bool flag)
{
    bool ok = (idx >= 1 && idx <= MAX_CLASSES);
    if (ok) objectClassPublished[idx-1] = flag;
    return ok;
}

bool INetIO::setObjectClassSubscribed(const unsigned int idx, const bool flag)
{
    bool ok = (idx >= 1 && idx <= MAX_CLASSES);
    if (ok) objectClassSubscribed[idx-1] = flag;
    return ok;
}

unsigned int INetIO::findObjectClassIndex(const RTI::ObjectClassHandle handle) const
{
    unsigned int index = 0;
    unsigned int max = getNumberOfObjectClasses();
    if (max > MAX_CLASSES) max = MAX_CLASSES;
    for (unsigned int i = 1; i <= max && index == 0; i++) {
        if (handle == objectClassHandles[i-1]) index = i;
    }
    return index;
}

void INetIO::clearAllObjectClassHandles()
{
    for (unsigned int i = 1; i <= MAX_CLASSES; i++) {
        setObjectClassHandle(i,0);
        setObjectClassPublished(i,false);
        setObjectClassSubscribed(i,false);
    }
}

//------------------------------------------------------------------------------
// HLA Object attribute handles
//    Note: attribute indexes range [ 1 ... MAX_ATTRIBUTES ]
//------------------------------------------------------------------------------
bool INetIO::setObjectAttributeHandle(const unsigned int idx, const RTI::AttributeHandle handle)
{
    bool ok = (idx >= 1 && idx <= MAX_ATTRIBUTES);
    if (ok) objectAttributeHandles[idx-1] = handle;
    return ok;
}

bool INetIO::setInteractionClassPublished(const unsigned int idx, const bool flag)
{
    bool ok = (idx >= 1 && idx <= MAX_INTERACTIONS);
    if (ok) interactionClassPublished[idx-1] = flag;
    return ok;
}

bool INetIO::setInteractionClassSubscribed(const unsigned int idx, const bool flag)
{
    bool ok = (idx >= 1 && idx <= MAX_INTERACTIONS);
    if (ok) interactionClassSubscribed[idx-1] = flag;
    return ok;
}

unsigned int INetIO::findAttributeIndex(const RTI::AttributeHandle handle) const
{
    unsigned int index = 0;
    unsigned int max = getNumberOfObjectAttributes();
    if (max > MAX_ATTRIBUTES) max = MAX_ATTRIBUTES;
    for (unsigned int i = 1; i <= max && index == 0; i++) {
        if (handle == objectAttributeHandles[i-1]) index = i;
    }
    return index;
}

void INetIO::clearAllObjectAttributeHandles()
{
    for (unsigned int i = 1; i <= MAX_ATTRIBUTES; i++) {
        setObjectAttributeHandle(i,0);
    }
}


//------------------------------------------------------------------------------
// HLA Interaction Class handles
//    Note: class indexes range [ 1 ... MAX_INTERACTIONS ]
//------------------------------------------------------------------------------
bool INetIO::setInteractionClassHandle(const unsigned int idx, const RTI::InteractionClassHandle handle)
{
    bool ok = (idx >= 1 && idx <= MAX_INTERACTIONS);
    if (ok) interactionClassHandles[idx-1] = handle;
    return ok;
}

unsigned int INetIO::findInteractionClassIndex(const RTI::InteractionClassHandle handle) const
{
    unsigned int index = 0;
    unsigned int max = getNumberOfOInteractionClasses();
    if (max > MAX_INTERACTIONS) max = MAX_INTERACTIONS;
    for (unsigned int i = 1; i <= max && index == 0; i++) {
        if (handle == interactionClassHandles[i-1]) index = i;
    }
    return index;
}

void INetIO::clearAllInteractionClassHandles()
{
    for (unsigned int i = 1; i <= MAX_INTERACTIONS; i++) {
        setInteractionClassHandle(i,0);
        setInteractionClassPublished(i,false);
        setInteractionClassSubscribed(i,false);
    }
}

//------------------------------------------------------------------------------
// HLA Interaction parameter handles
//    Note: range [ 1 ... MAX_PARAMETERS ]
//------------------------------------------------------------------------------
bool INetIO::setInteractionParameterHandle(const unsigned int idx, const RTI::ParameterHandle handle)
{
    bool ok = (idx >= 1 && idx <= MAX_PARAMETERS);
    if (ok) interactionParameterHandles[idx-1] = handle;
    return ok;
}

unsigned int INetIO::findParameterIndex(const RTI::ParameterHandle handle) const
{
    unsigned int index = 0;
    unsigned int max = getNumberOfInteractionParameters();
    if (max > MAX_PARAMETERS) max = MAX_PARAMETERS;
    for (unsigned int i = 1; i <= max && index == 0; i++) {
        if (handle == interactionParameterHandles[i-1]) index = i;
    }
    return index;
}

void INetIO::clearAllInteractionParameterHandles()
{
    for (unsigned int i = 1; i <= MAX_PARAMETERS; i++) {
        setInteractionParameterHandle(i,0);
    }
}

//------------------------------------------------------------------------------
// HLA Object Class registration enabled flags
//------------------------------------------------------------------------------
bool INetIO::setObjectClassRegistrationFlag(const unsigned int idx, const bool flg)
{
   bool ok = (idx >= 1 && idx <= MAX_CLASSES);
   if (ok) regEnbl[idx-1] = flg;
   return ok;
}

void INetIO::clearAllObjectClassRegistrationFlags()
{
   for (unsigned int i = 1; i <= MAX_CLASSES; i++) {
      setObjectClassRegistrationFlag(i,false);
   }
}

//------------------------------------------------------------------------------
// HLA Interaction Class enabled flags
//------------------------------------------------------------------------------
bool INetIO::setInteractionEnabledFlag(const unsigned int idx, const bool flg)
{
   bool ok = (idx >= 1 && idx <= MAX_INTERACTIONS);
   if (ok) interactEnbl[idx-1] = flg;
   return ok;
}

void INetIO::clearAllInteractionEnabledFlags()
{
   for (unsigned int i = 1; i <= MAX_INTERACTIONS; i++) {
      setInteractionEnabledFlag(i,false);
   }
}

//------------------------------------------------------------------------------
// unregisterAllObjects() --
//------------------------------------------------------------------------------
bool INetIO::unregisterAllObjects()
{
   // Stop Registration For Object Classes
   clearAllObjectClassRegistrationFlags();

   // Unregister all of our output objects ...
   for (unsigned int idx = 0; idx < nOutObjects; idx++) {
      INib* nib = outHandleTbl[idx];
      if (nib->isRegistered()) {
         getRTIambassador()->deleteObjectInstance(nib->getObjectHandle(),0);
         nib->setObjectHandle(0);
         nib->setClassIndex(0);
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// unPublishAndSubscribe()
//------------------------------------------------------------------------------
bool INetIO::unPublishAndSubscribe()
{
   bool ok = true;
   try {
      // Un-publish and un-subscribe all object classes
      for (unsigned int i = 1; i <= MAX_CLASSES; i++) {
         RTI::ObjectClassHandle handle = getObjectClassHandle(i);
         if (handle > 0) {
            if (isObjectClassPublished(i)) {
               getRTIambassador()->unpublishObjectClass(handle);
               setObjectClassPublished(i,false);
            }
            if (isObjectClassSubscribed(i)) {
               getRTIambassador()->unsubscribeObjectClass(handle);
               setObjectClassSubscribed(i,false);
            }
         }
      }

      // Un-publish and un-subscribe all interaction classes
      for (unsigned int i = 1; i <= MAX_INTERACTIONS; i++) {
         RTI::InteractionClassHandle handle = getInteractionClassHandle(i);
         if (handle > 0) {
            if (isInteractionClassPublished(i)) {
               getRTIambassador()->unpublishInteractionClass(handle);
               setInteractionClassPublished(i,false);
            }
            if (isInteractionClassSubscribed(i)) {
               getRTIambassador()->unsubscribeInteractionClass(handle);
               setInteractionClassSubscribed(i,false);
            }
         }
      }
   }
   catch(RTI::Exception& e) {
      std::cout << &e << std::endl;
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// createFederateAmbassador() -- Create our ambassador
//------------------------------------------------------------------------------
Ambassador* INetIO::createFederateAmbassador()
{
    return new Ambassador(this);
}

//------------------------------------------------------------------------------
// getFedFileName() -- Return a pointer to the FED filename
//------------------------------------------------------------------------------
const char* INetIO::getFedFileName() const
{
    const char* s{};
    if (fedFileName != nullptr) {
        s = fedFileName->c_str();
    }
    return s;
}

//------------------------------------------------------------------------------
// updateAttributeValues() -- Send attributes to the RTI
//------------------------------------------------------------------------------
bool INetIO::updateAttributeValues(const RTI::ObjectHandle handle, RTI::AttributeHandleValuePairSet* attrs, const char* theTag)
{
    bool ok = true;
    if ( attrs->size() > 0) {
        try  {
            if (getRegulating()) {
                //DPG getRTIambassador()->queryLookahead(getLookAhead());
                getRTIambassador()->updateAttributeValues(handle, *attrs, (getFederationTime()+getLookAhead()), theTag);
            }
            else {
                getRTIambassador()->updateAttributeValues(handle, *attrs, 0);
            }
        }
        catch (RTI::Exception& e) {
            std::cerr << &e << std::endl;
            ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// sendInteraction() -- Send the interaction via the RTI ambassador
//------------------------------------------------------------------------------
bool INetIO::sendInteraction(const RTI::InteractionClassHandle handle, RTI::ParameterHandleValuePairSet* pParams, const char* theTag)
{
    bool ok = true;
    try {
        getRTIambassador()->sendInteraction(handle, *pParams, theTag);
    }
    catch (RTI::Exception& e) {
        std::cerr << &e << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// netInputHander() -- Network input handler
//------------------------------------------------------------------------------
void INetIO::netInputHander()
{
    doTick();
}

//------------------------------------------------------------------------------
// initNetwork() -- Initialize the multicast network
//------------------------------------------------------------------------------
bool INetIO::initNetwork()
{
    // ---
    // Create the federate unique ambassador
    // ---
    fedAmb = createFederateAmbassador();
    bool ok = (fedAmb != nullptr);

    // ---
    // join the federation
    // ---
    if (ok) {
        ok = createAndJoinFederation();
        doTick();
    }

    // initialize time constraints, etc.
    //if (getRegulating() || getConstrained())
    //{
    //    setLookAhead(deltaTime);
    //    setTimeIncrement(deltaTime);
    //    setFederationTime(0.0);
    //    initializeTimeManagement();
    //    doTick();
    //}

    if (ok) {
        ok = publishAndSubscribe();
        doTick();
    }

    return true;
}


//------------------------------------------------------------------------------
// createAndJoinFederation()
//------------------------------------------------------------------------------
bool INetIO::createAndJoinFederation()
{
    bool ok{};

    const std::string& federation{getFederationName()};
    const std::string& federate{getFederateName()};

    if ( !federation.empty() && !federate.empty() ) {

       ok = true; // default

       // ---
       // Try to create the federation
       // ---
       try {
           rtiAmb->createFederationExecution(federation.c_str(), getFedFileName());
           std::cout << "*** Federation Created" << std::endl;
           base::msleep(1000);   // DDH: this was set to 2000 for linux?
       }
       catch (RTI::FederationExecutionAlreadyExists& e) {
           std::cout << &e << std::endl;
       }
       catch ( RTI::Exception& e) {
           std::cerr << &e << std::endl;
           ok = false;
       }

       // ---
       // Try to join the federation.
       // ---
       if (ok) {
           RTI::Boolean joined{RTI::RTI_FALSE};
           int tries{10};
           while (tries--) {
               try {
                   rtiAmb->joinFederationExecution(federate.c_str(),
                                                   federation.c_str(),
                                                   fedAmb);
                   std::cout << "*** Joined Federation" << std::endl;
                   joined = RTI::RTI_TRUE;
                   break;
               }
               catch (RTI::FederationExecutionDoesNotExist) {
                   base::msleep(1000);   // linux might want 2000
               }
               catch (RTI::Exception& e) {
                   std::cout << &e << std::endl;
               }
           }
           if (!joined) {
               std::cout << "Couldn't Join the Federation" << std::endl;
               ok = false;
           }
       }
    }

    return ok;
}

//------------------------------------------------------------------------------
// resignAndDestroyFederation()
//------------------------------------------------------------------------------
bool INetIO::resignAndDestroyFederation()
{
    const std::string&  federation{getFederationName()};

    if ( !federation.empty()) {
       try {
           rtiAmb->resignFederationExecution(
               RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
               std::cout << "*** Resigned Federation" << std::endl;
       }
       catch(RTI::Exception& e) {
           std::cerr << &e << std::endl;
           return false;
       }

       try {
           rtiAmb->destroyFederationExecution(federation.c_str());
           std::cout << "*** Destroyed Federation" << std::endl;
       }
       catch(RTI::Exception& e) {
           std::cerr << &e << std::endl;
       }
    }

    return true;
}

//------------------------------------------------------------------------------
// initializeTimeManagement() -- DPG -- Need to finish this code!!
//------------------------------------------------------------------------------
bool INetIO::initializeTimeManagement()
{
   bool ok = true;

   // When not regulating or constrained, it the default so just leave
   if ( !getRegulating() && !getConstrained() ) {
      std::cout << "*** Time Regulation Turned Off" << std::endl;
      std::cout << "*** Time Constraint Turned Off" << std::endl;
   }

   // If this federate is regulating and not constrained, then we
   // want to set cTime to the FederateTime.  Otherwise, we
   // will throw exceptions when posting messages.
   else if ( getRegulating() && !getConstrained() ) {
      try {
         std::cout << "*** Time Regulation Turned On" << std::endl;
         //DPG rtiAmb.enableTimeRegulation(getFederationTime(), getLookAhead());

         std::cout << "*** Time Constraint Turned Off" << std::endl;

         //DPG rtiAmb.queryFederateTime(getFederationTime());
         std::cout << "  *** CurrentTime: " << getFederationTime() << std::endl;
      }
      catch (RTI::Exception& e) {
         std::cout << &e << std::endl;
         ok = false;
      }
   }

   // If this federate is not regulating and constrained, then we
   // want to set cTime to the LBTS.  While the only requirement
   // is that the federate not advance past the LBTS, we want this
   // federate to start with the others.  If the LBTS is positive infinity,
   // then there are no regulating federates.  We should then set
   // cTime to 0 to avoid hanging in while advancing time.
   else if ( !getRegulating() && getConstrained() ) {
      try {
         std::cout << "*** Time Regulation Turned Off" << std::endl;

         std::cout << "*** Time Constraint Turned On" << std::endl;
         //DPG rtiAmb.enableTimeConstrained();

         //DPG rtiAmb.queryLBTS(getFederationTime());
         //DPG if ( getFederationTime().isPositiveInfinity() ) {
            //DPG setFederationTime(0.0);
         //DPG }
         std::cout << "  *** CurrentTime: " << getFederationTime() << std::endl;
      }
      catch (RTI::Exception& e) {
         std::cout << &e << std::endl;
         ok = false;
      }
   }

   // If we are both regulating and constrained, then the federate
   // should set cTime to FederateTime.  This value will be
   // less than or equal to the LBTS, so the constrained conditions
   // will be satisfied.
   else {
      try {
         std::cout << "*** Time Regulation Turned On" << std::endl;
         //DPG rtiAmb.enableTimeRegulation(getFederationTime(), getLookAhead());

         std::cout << "*** Time Constraint Turned On" << std::endl;
         //DPG rtiAmb.enableTimeConstrained();

         //DPG rtiAmb.queryFederateTime(getFederationTime());
         std::cout << "  *** CurrentTime: " << getFederationTime() << std::endl;
      }
      catch (RTI::Exception& e) {
         std::cout << &e << std::endl;
         ok = false;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// advanceTime() -- DPG -- Need to finish this code!!
//------------------------------------------------------------------------------
bool INetIO::advanceTime()
{
   // When not regulating or constrained, just leave
   if ( !getRegulating() && !getConstrained() ) return true;

   if ( !getTimeAdvanceOutstanding() ) {
      //try {
//         std::cout << " timeAdvanceRequest:" << GetFederationTime() << " + " << GetTimeIncrement() << std::endl;
         //DPG rtiAmb.timeAdvanceRequest( getFederationTime() + getTimeIncrement() );
         //DPG setTimeAdvanceOutstanding(RTI::RTI_TRUE);

         //DPG while ( getTimeAdvanceOutstanding() ) {
//            std::cout << "*** Ticking..." << std::endl;
            //rtiAmb.tick(0.0, 1.0);
            //DPG rtiAmb.tick();  // TESTING
         //DPG }
      //}
      //catch ( RTI::Exception& e) {
      //   std::cout << "Time advance error: " << &e << std::endl;
      //   return false;
      //}
   }
   return true;
}


//------------------------------------------------------------------------------
// doTick()
//------------------------------------------------------------------------------
bool INetIO::doTick()
{
   bool ok = true;
   //  std::cout << "*** Ticking..." << std::endl;
   try {
      //rtiAmb.tick(0.0,1.0);
      rtiAmb->tick();
   }
   catch (RTI::Exception& e) {
      std::cerr << "Exception throw from the RTI during tick: " << &e << std::endl;
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set slot routines
//------------------------------------------------------------------------------
bool INetIO::setSlotFedFile(base::String* const msg)
{
   fedFileName = msg;
   return true;
}

bool INetIO::setSlotRegulatingTime(base::Boolean* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      if (msg->asBool()) setRegulating(RTI::RTI_TRUE);
      else setRegulating(RTI::RTI_FALSE);
      ok = true;
   }
   return ok;
}

bool INetIO::setSlotConstrainedTime(base::Boolean* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      if (msg->asBool()) setConstrained(RTI::RTI_TRUE);
      else setConstrained(RTI::RTI_FALSE);
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// addNibToObjectTables() -- adds a NIB to the quick access object tables
//------------------------------------------------------------------------------
void INetIO::addNibToObjectTables(interop::INib* const nib, const IoType ioType)
{
   const auto hlaNib = dynamic_cast<INib*>(nib);

   if (hlaNib != nullptr && nInObjects < MAX_OBJECTS) {
      // Add to the 'by object name' and 'by object handle' tables
      if (ioType == INPUT_NIB ) {
         addNibToNameTable(hlaNib, inNameTbl.data(), nInObjects);
         addNibToHandleTable(hlaNib, inHandleTbl.data(), nInObjects);
         nInObjects++;
      }
      else if (ioType == OUTPUT_NIB) {
         addNibToNameTable(hlaNib, outNameTbl.data(), nOutObjects);
         addNibToHandleTable(hlaNib, outHandleTbl.data(), nOutObjects);
         nOutObjects++;
      }
   }
}

void INetIO::addNibToNameTable(INib* const nib, INib** tbl, const unsigned int n)
{
   // Put the NIB on the top of the table
   nib->ref();
   tbl[n] = nib;

   if (n > 0) {
      // Now, 'bubble down' to it's correct position
      unsigned int idx = n-1;
      while (idx >= 0 && compareObjNames(nib->getObjectName(), &tbl[idx]) <= 0) {
         // Swap the table entries
         INib* tmp = tbl[idx];
         tbl[idx] = tbl[idx+1];
         tbl[idx+1] = tmp;
         idx--;
      }
   }
}

void INetIO::addNibToHandleTable(INib* const nib, INib** tbl, const unsigned int n)
{
   // Put the NIB on the top of the table
   nib->ref();
   tbl[n] = nib;

   if (n > 0) {
      // Now, 'bubble down' to it's correct position
      const RTI::ObjectClassHandle handle = nib->getObjectHandle();
      unsigned int idx = n-1;
      while (idx >= 0 && compareObjHandles(&handle, &tbl[idx]) <= 0) {
         // Swap the table entries
         INib* tmp = tbl[idx];
         tbl[idx] = tbl[idx+1];
         tbl[idx+1] = tmp;
         idx--;
      }
   }
}

//------------------------------------------------------------------------------
// removeNibFromObjectTables() -- removes a NIB to the quick access object tables
//------------------------------------------------------------------------------
void INetIO::removeNibFromObjectTables(interop::INib* const nib, const IoType ioType)
{
   const auto hlaNib = dynamic_cast<INib*>(nib);

   if (hlaNib != nullptr && nInObjects > 0) {
      // Remove from the 'by object name' and 'by object handle' tables
      if (ioType == INPUT_NIB) {
         removeNibFromTable(hlaNib, inNameTbl.data(), nInObjects);
         removeNibFromTable(hlaNib, inHandleTbl.data(), nInObjects);
         nInObjects--;
      }
      else if (ioType == OUTPUT_NIB) {
         removeNibFromTable(hlaNib, outNameTbl.data(), nOutObjects);
         removeNibFromTable(hlaNib, outHandleTbl.data(), nOutObjects);
         nOutObjects--;
      }
   }
}

void INetIO::removeNibFromTable(INib* const nib, INib** tbl, const unsigned int n)
{
   int found = -1;
   // Find the NIB
   for (unsigned int i = 0; i < n && found < 0; i++) {
      if (nib == tbl[i]) found = i;
   }

   // Shift down all items above this NIB one position
   if (found >= 0) {
      tbl[found]->unref();
      unsigned int n1 = (n - 1);
      for (unsigned int i = found; i < n1; i++) {
         tbl[i] = tbl[i+1];
      }
      tbl[n-1] = nullptr;
   }
}

//------------------------------------------------------------------------------
// findNibByObjectHandle() -- find the NIB that matches the Object Handle
//------------------------------------------------------------------------------
INib* INetIO::findNibByObjectHandle(RTI::ObjectHandle handle, const IoType ioType)
{
   INib* found = nullptr;
   if (ioType == INPUT_NIB) {
      INib** k = static_cast<INib**>(bsearch(&handle, inHandleTbl.data(), nInObjects, sizeof(INib*), compareObjHandles));
      if (k != nullptr) found = *k;
   }
   else {
      INib** k = static_cast<INib**>(bsearch(&handle, outHandleTbl.data(), nOutObjects, sizeof(INib*), compareObjHandles));
      if (k != nullptr) found = *k;
   }
   return found;
}

//------------------------------------------------------------------------------
// findNibByObjectName() -- find the NIB that matches the Object's fed name
//------------------------------------------------------------------------------
INib* INetIO::findNibByObjectName(const char* name, const IoType ioType)
{
   INib* found = nullptr;
   if (ioType == INPUT_NIB) {
      INib** k = static_cast<INib**>(bsearch(name, inNameTbl.data(), nInObjects, sizeof(INib*), compareObjNames));
      if (k != nullptr) found = *k;
   }
   else {
      INib** k = static_cast<INib**>(bsearch(name, outNameTbl.data(), nOutObjects, sizeof(INib*), compareObjNames));
      if (k != nullptr) found = *k;
   }
   return found;
}

//------------------------------------------------------------------------------
// Destroy the NIB s
//------------------------------------------------------------------------------
void INetIO::destroyInputNib(interop::INib* const nib)
{
   std::cout << "NetIO::destroyInputNib(" << nib << ")" << std::endl;
   if (nib != nullptr) {
      // Remove it from our object name and handle tables
      removeNibFromObjectTables(nib, interop::INetIO::INPUT_NIB);
      // Let our base class handle the rest
      BaseClass::destroyInputNib(nib);
   }
}

void INetIO::destroyOutputNib(interop::INib* const nib0)
{
   std::cout << "NetIO::destroyOutputNib(" << nib0 << ")" << std::endl;
   const auto nib = dynamic_cast<INib*>(nib0);
   if (nib != nullptr) {
      if (nib->isRegistered()) {
         // When this output NIB was registered as an HLA object ...
         std::cout << "###DeleteReq- Unref-2: handle: " << nib->getObjectHandle() << std::endl;
         removeNibFromObjectTables(nib, OUTPUT_NIB);
         getRTIambassador()->deleteObjectInstance(nib->getObjectHandle(), nullptr);
         nib->setObjectHandle(0);
         nib->setClassIndex(0);
      }
      // Let our base class handle the rest
      BaseClass::destroyOutputNib(nib);
   }
}

//------------------------------------------------------------------------------
// bsearch callbacks:
//
//   returns  <0 if key p1 is less than object p2
//        or   0 if key p1 is equal to object p2
//        or  >0 if key p1 is greater than object p2
//------------------------------------------------------------------------------

// compareObjNames() -- object name compare function --
//   True types are (const char* p1, const Nib** p2)
int INetIO::compareObjNames(const void* p1, const void* p2)
{
   // Key's name
   const char* name1 = static_cast<const char*>(p1);

   // NIB's name
   const INib* pNib = *((const INib**) p2);
   const char* name2 = pNib->getObjectName();

   // compare the names
   int result = std::strcmp(name1, name2);
   return result;
}

// compareObjHandles() --  object handle compare function --
//   True types are (const RTI::ObjectClassHandle* p1, const Nib** p2)
int INetIO::compareObjHandles(const void* p1, const void* p2)
{
   // Key's handle
   const RTI::ObjectClassHandle h1 = *static_cast<const RTI::ObjectClassHandle*>(p1);

   // NIB's handle
   const INib* pNib = *((const INib**) p2);
   const RTI::ObjectClassHandle h2 = pNib->getObjectHandle();

   // Compare the handles
   int result = 0;
   if (h1 > h2) result = +1;
   else if (h1 < h2) result = -1;

   return result;
}

}
}

