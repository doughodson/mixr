
#include "mixr/base/IObject.hpp"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace mixr {
namespace base {

// ---
// Class and object metadata
// ---
MetaObject IObject::metaObject(typeid(IObject).name(), "IObject", &IObject::slottable, nullptr);

// ---
// IObject's SlotTable
//    Note: Keep our slot table empty.  There are several light weight classes derived
//    from Object (e.g., the classes for colors, numbers and qty) that expect their
//    slots to be the first slots -- starting at slot number 1.
// ---
const char* IObject::slotnames[] = { "" };
const int IObject::nslots{};
const SlotTable IObject::slottable(nullptr, 0);

//------------------------------------------------------------------------------
// Standard object stuff -- derived classes used macro IMPLEMENT_SUBCLASS, see macros.hpp
//------------------------------------------------------------------------------

IObject::IObject():IReferenced()
{
   STANDARD_CONSTRUCTOR()
}

IObject::IObject(const IObject& org):IReferenced()
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

IObject::~IObject()
{
   STANDARD_DESTRUCTOR()
}

IObject& IObject::operator=(const IObject& org)
{
    if (this != &org) copyData(org);
    return *this;
}

// Check class type
bool IObject::isClassType(const std::type_info& type) const
{
    if ( type == typeid(IObject) ) return true;
    else return false;
}

// Check factory name
bool IObject::isFactoryName(const std::string& name) const
{
    if (name.empty()) return false;
    if ( metaObject.getFactoryName() == name)  return true;
    else return false;
}

// Copy object data -- derived classes should call
// BaseClass::copyData() first and then copy their data.
void IObject::copyData(const IObject& org, const bool cc)
{
    slotTable = org.slotTable;
    enbMsgBits = org.enbMsgBits;
    disMsgBits = org.disMsgBits;
}

// Delete object data -- derived classes should delete
// or unref() their own data
void IObject::deleteData()
{
}

// set slots by index
bool IObject::setSlotByIndex(const int, IObject* const)
{
    // We have no slots, so we shouldn't ever be here!
    return false;
}

const std::string& IObject::getFactoryName()
{
    return metaObject.getFactoryName();
}

const SlotTable& IObject::getSlotTable()
{
   return slottable;
}

//------------------------------------------------------------------------------
// slotName2Index() -- returns the index of the slot named 'slotname'
//------------------------------------------------------------------------------
int IObject::slotName2Index(const char* const slotname) const
{
   int slotindex{};

   // No 'slotname' then no slot index
   if (slotname == nullptr) {
      return slotindex;
   }

   // How many slots do we have
   int n{slotTable->n()};

   // a) check if 'slotname' is a number (e.g., "12")
   bool isNum{true};
   for (int i = 0; isNum && slotname[i] != '\0'; i++) {
      if ( !std::isdigit(slotname[i]) ) {
         isNum = false;
      }
   }

   // b) convert 'slotname' to a slot index
   if (isNum) {
      // when the slotname is just a number (e.g., "12")
      int j{std::atoi(slotname)};
      if (j > 0 && j <= n) {
         slotindex = j;
      }
   } else {
      // when the 'slotname' is a name (e.g., "some-slot")
      slotindex = slotTable->index(slotname);
      if (slotindex <= 0)
         std::cerr << "slot not found: " << slotname << std::endl;
   }
   return slotindex;
}

//------------------------------------------------------------------------------
// setSlotByName() -- set the value of slot 'slotname' to 'obj'  Returns
//                 true if the slot and object were processed; returns
//                 false if there was an error.
//------------------------------------------------------------------------------
bool IObject::setSlotByName(const char* const slotname, IObject* const obj)
{
    bool ok{};
    if (obj == nullptr) return ok;
    const int slotindex{slotName2Index(slotname)};
    if (slotindex > 0) {
        ok = setSlotByIndex(slotindex,obj);
    }
    return ok;
}

//------------------------------------------------------------------------------
// slotIndex2Name() -- returns the name of the slot at 'slotindex'
//------------------------------------------------------------------------------
const char* IObject::slotIndex2Name(const int slotindex) const
{
   return slotTable->name(slotindex);
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid object
//------------------------------------------------------------------------------
bool IObject::isValid() const
{
   return true;
}

//------------------------------------------------------------------------------
// Message types
//------------------------------------------------------------------------------

bool IObject::isMessageEnabled(const unsigned short msgType) const
{
   bool enabled{};

   if (msgType != 0) {
      if ((msgType & MSG_ERROR) != 0) {
         // Error messages are always enabled
         enabled = true;
      } else {
         // enabled if any bits match
         enabled = (msgType & enbMsgBits) != 0;
      }
   }
   return enabled;
}

bool IObject::isMessageDisabled(const unsigned short msgType) const
{
   bool disabled{};

   if (msgType != 0) {
      if ((msgType & MSG_ERROR) != 0) {
         // Error messages are not disabled
         disabled = false;
      } else {
         // disabled if any bits match
         disabled = (msgType & disMsgBits) != 0;
      }
   }

   return disabled;
}

bool IObject::enableMessageTypes(const unsigned short msgTypeBits)
{
   // Set the enabled bits
   enbMsgBits |= msgTypeBits;

   // Clear the disabled bits
   disMsgBits &= ~msgTypeBits;

   return true;
}

bool IObject::disableMessageTypes(const unsigned short msgTypeBits)
{
   // Set the disabled bits
   disMsgBits |= msgTypeBits;

   // Clear the enabled bits
   enbMsgBits &= ~msgTypeBits;

   return true;
}

//------------------------------------------------------------------------------
// return object and class metadata
//------------------------------------------------------------------------------
const MetaObject* IObject::getMetaObject()
{
   return &metaObject;
}

}
}

