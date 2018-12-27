
#ifndef __mixr_base_Pair_H__
#define __mixr_base_Pair_H__

#include "mixr/base/Object.hpp"
//#include "mixr/base/Identifier.hpp"

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
class Pair : public Object
{
   DECLARE_SUBCLASS(Pair, Object)

public:
   // Constructor: the slot name and object pointer are both required!
   // -- the object is ref() by this constructor.
   Pair(const std::string& slot, Object* object);

   std::string& slot()              { return slotname; } // The slot name
   const std::string& slot() const  { return slotname; } // The slot name (const version)

   Object* object()                 { return obj; }      // The object
   const Object* object() const     { return obj; }      // The object (const version)

   bool isValid() const override;

private:
   std::string slotname;     // Slot name
   Object* obj{};            // Object
};

}
}

#endif
