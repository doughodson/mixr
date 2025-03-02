
#ifndef __mixr_base_Pair_HPP__
#define __mixr_base_Pair_HPP__

#include "mixr/base/IObject.hpp"

#include <string>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Pair
// Description: Slot pair (or named object); i.e., a std::string name/Object pair.
//
//    Used to set object attributes, i.e., 'slots' (see Object::setSlotByName()).
//
//    Used to name objects, since the Object class does not have a name field,
//    and since objects can be in more than one Pair, it can have more than one
//    name (aliases).
//
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Pair
// Slots: none
// Notes: EDL syntax (an Identifier followed by a colon then Object)
//      ident: <object>
//------------------------------------------------------------------------------
class Pair : public IObject
{
   DECLARE_SUBCLASS(Pair, IObject)

public:
   // Constructor: the slot name and object pointer are both required!
   // -- the object is ref() by this constructor.
   Pair(const std::string& slot, IObject* object);

   std::string& slot()              { return slotname; } // The slot name
   const std::string& slot() const  { return slotname; } // The slot name (const version)

   IObject* object()                { return obj; }      // The object
   const IObject* object() const    { return obj; }      // The object (const version)

   bool isValid() const override;

private:
   std::string slotname;     // Slot name
   IObject* obj{};           // Object
};

}
}

#endif
