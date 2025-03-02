
#ifndef __mixr_base_numeric_Boolean_HPP__
#define __mixr_base_numeric_Boolean_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Boolean
// Description: A data type that represents two values (true and false).  This
//              class is defined to support the translation of Boolean type
//              values from the Extensible Description Language (EDL) to C++
//              code that consumes that specification.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Boolean
// Slots:
//     value  <Boolean>  ! Sets the value (default: false)
//------------------------------------------------------------------------------
class Boolean final: public IObject
{
   DECLARE_SUBCLASS(Boolean, IObject)

public:
   explicit Boolean()                  { STANDARD_CONSTRUCTOR()            }
   explicit Boolean(const bool x)      { STANDARD_CONSTRUCTOR() value = x; }

   void setValue(const bool x)         { value = x;    }
   bool asBool() const                 { return value; }

private:
   bool value{};

private:
   // slot table helper methods
   virtual bool setSlotValue(const Boolean* const);
};

}
}

#endif
