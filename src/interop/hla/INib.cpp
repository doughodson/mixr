
#include "mixr/interop/INetIO.hpp"
#include "mixr/interop/hla/INib.hpp"
#include "mixr/interop/hla/INetIO.hpp"
#include "mixr/interop/hla/Ambassador.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/Pair.hpp"

#include <cstdio>
#include <string>

namespace mixr {
namespace hla {

IMPLEMENT_PARTIAL_SUBCLASS(INib, "HlaNib")
EMPTY_SLOTTABLE(INib)
EMPTY_DELETEDATA(INib)

INib::INib(const interop::INetIO::IoType ioType) : interop::INib(ioType), oname()
{
   STANDARD_CONSTRUCTOR()
   setTimeoutEnabled(true);
   clearAllAttributeUpdateEnabledFlags();
   clearAllAttributeUpdateRequiredFlags();
}

INib::INib(const INib& org) : interop::INib(org.getIoType())
{
   STANDARD_CONSTRUCTOR()
   copyData(org,true);
}

INib::~INib()
{
   STANDARD_DESTRUCTOR()
}

INib& INib::operator=(const INib& org)
{
   deleteData();
   copyData(org,false);
   return *this;
}

INib* INib::clone() const
{
   return new INib(*this);
}

void INib::copyData(const INib& org, const bool)
{
   BaseClass::copyData(org);
   oname = org.oname;
   handle = org.handle;
   objectClassIndex = org.objectClassIndex;

   for (unsigned int i = 0; i < INetIO::MAX_ATTRIBUTES; i++) {
      updateEnabled[i] = org.updateEnabled[i];
      updateRequired[i] = org.updateRequired[i];
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void INib::setObjectHandle(RTI::ObjectHandle h)
{
   handle = h;
}

void INib::setClassIndex(const unsigned int idx)
{
   objectClassIndex = idx;
}

void INib::setObjectName(const char* s)
{
   oname = s;
}

// Makes a default RTI name
void INib::makeObjectName()
{
   char name[256]{};
   const std::string& fname{getFederateName()};
   std::sprintf(name, "P%d_%s", static_cast<int>(getPlayerID()), fname.c_str() );
   setObjectName(name);
}

//------------------------------------------------------------------------------
// reflectAttributeValues() -- (Input support)
//  Called by our FederateAmbassador to update the attribute values for
//  this object instance.
//------------------------------------------------------------------------------
void INib::reflectAttributeValues(const RTI::AttributeHandleValuePairSet&)
{
   //std::cout << "Nib::reflectAttributeValues() here!" << std::endl;
}

//------------------------------------------------------------------------------
// HLA attribute update required flags
//------------------------------------------------------------------------------
void INib::setAttributeUpdateRequiredFlag(const unsigned int attribIndex, const bool flg)
{
   if (attribIndex >= 1 && attribIndex <= INetIO::MAX_ATTRIBUTES) updateRequired[attribIndex-1] = flg;
}

void INib::setAllAttributeUpdateRequiredFlags()
{
   for (unsigned int i = 1; i <= INetIO::MAX_ATTRIBUTES; i++) {
      setAttributeUpdateRequiredFlag(i, true);
   }
}

void INib::clearAllAttributeUpdateRequiredFlags()
{
   for (unsigned int i = 1; i <= INetIO::MAX_ATTRIBUTES; i++) {
      setAttributeUpdateRequiredFlag(i, false);
   }
}

//------------------------------------------------------------------------------
// HLA attribute update enabled flags
//------------------------------------------------------------------------------
void INib::setAttributeUpdateEnabledFlag(const unsigned int attribIndex, const bool flg)
{
   if (attribIndex >= 1 && attribIndex <= INetIO::MAX_ATTRIBUTES) updateEnabled[attribIndex-1] = flg;
}

void INib::clearAllAttributeUpdateEnabledFlags()
{
   for (unsigned int i = 1; i <= INetIO::MAX_ATTRIBUTES; i++) {
      setAttributeUpdateEnabledFlag(i, false);
   }
}

//------------------------------------------------------------------------------
// isPlayerStateUpdateRequiredForOutput() -- check to see if an update is required
//------------------------------------------------------------------------------
bool INib::isPlayerStateUpdateRequired(const double curExecTime)
{
    if (!isRegistered()) return true;
    else return BaseClass::isPlayerStateUpdateRequired(curExecTime);
}

//------------------------------------------------------------------------------
// turnUpdatesOn() -- (Output support)
//  Called by our FederateAmbassador to handle the turnUpdatesOnForObjectInstance()
//  callbacks for this object instance.
//------------------------------------------------------------------------------
void INib::turnUpdatesOn(const RTI::AttributeHandleSet& theAttributes)
{
   INetIO* hlaIO {static_cast<INetIO*>(getNetIO())};
   if (hlaIO != nullptr) {
      //std::cout << getObjectName();
      //std::cout << " ON ( ";
      for (RTI::ULong i = 0; i < theAttributes.size(); i++) {
         RTI::AttributeHandle theHandle {theAttributes.getHandle(i)};
         unsigned int theIndex {hlaIO->findAttributeIndex(theHandle)};
         //std::cout << "[" << theHandle << ":";
         if (theIndex != 0) {
            //std::cout << theIndex;
            setAttributeUpdateEnabledFlag(theIndex, true);
         }
         //std::cout << "] ";
      }
      //std::cout << ")";
   }
}

//------------------------------------------------------------------------------
// turnUpdatesOff() -- (Output support)
//  Called by our FederateAmbassador to handle the turnUpdatesOffForObjectInstance()
//  callbacks for this object instance.
//------------------------------------------------------------------------------
void INib::turnUpdatesOff(const RTI::AttributeHandleSet& theAttributes)
{
   INetIO* hlaIO {static_cast<INetIO*>(getNetIO())};
   if (hlaIO != nullptr) {
      //std::cout << getObjectName();
      //std::cout << " OFF ( ";
      for (RTI::ULong i = 0; i < theAttributes.size(); i++) {
         RTI::AttributeHandle theHandle {theAttributes.getHandle(i)};
         unsigned int theIndex {hlaIO->findAttributeIndex(theHandle)};
         //std::cout << "[" << theHandle << ":";
         if (theIndex != 0) {
            //std::cout << theIndex;
            setAttributeUpdateEnabledFlag(theIndex, false);
         }
         //std::cout << "] ";
      }
      //std::cout << ")";
   }
}

//------------------------------------------------------------------------------
// provideAttributeValueUpdate() -- (Output support)
//  Called by our FederateAmbassador to request attribute value updates
//------------------------------------------------------------------------------
void INib::provideAttributeValueUpdate(const RTI::AttributeHandleSet& theAttributes)
{
   INetIO* hlaIO {static_cast<INetIO*>(getNetIO())};
   if (hlaIO != nullptr) {
      //std::cout << getObjectName();
      //std::cout << " Update ( ";
      for (RTI::ULong i = 0; i < theAttributes.size(); i++) {
         RTI::AttributeHandle theHandle {theAttributes.getHandle(i)};
         unsigned int theIndex {hlaIO->findAttributeIndex(theHandle)};
         //std::cout << "[" << theHandle << ":";
         if (theIndex != 0) {
            //std::cout << theIndex;
            setAttributeUpdateRequiredFlag(theIndex, false);
         }
         //std::cout << "] ";
      }
      //std::cout << ")";
   }
}

}
}

