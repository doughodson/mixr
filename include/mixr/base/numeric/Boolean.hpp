
#ifndef __mixr_base_numeric_Boolean_H__
#define __mixr_base_numeric_Boolean_H__

#include "mixr/base/Object.hpp"

#include <iostream>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Boolean
//
// Description: A data type that represents two values (true and false).  This
//              class is defined to support the translation from an Extensible
//              Description Language (EDL) input configuration Boolean value
//              type to the equivalent C++ type (i.e., 'bool'). 
//
// Factory name: Boolean
// Slots:
//     value  <Boolean>  ! Sets the value. (default: false)
//
//------------------------------------------------------------------------------
class Boolean : public Object
{
   DECLARE_SUBCLASS(Boolean, Object)

public:
   Boolean(const bool x)               { STANDARD_CONSTRUCTOR() value = x; }
   Boolean()                           { STANDARD_CONSTRUCTOR()            }

   operator bool() const               { return value;            }   // conversion
   Boolean& operator=(const bool x)    { value = x; return *this; }   // assignment

   bool getBoolean() const             { return value; }

private:
   bool value{};

private:
   // slot table helper methods
   virtual bool setSlotValue(const Boolean* const);
};

}
}

#endif
