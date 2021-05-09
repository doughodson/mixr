
#ifndef __mixr_base_numeric_Number_HPP__
#define __mixr_base_numeric_Number_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Number
// Description:  Abstract class for number-based objects (Float, Integer) and 
//               numerical operators (Add, Subtract, Multiply and Divide)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: AbstractNumber
// Slots: none
//------------------------------------------------------------------------------
class Number : public Object
{
   DECLARE_SUBCLASS(Number, Object)

public:
   explicit Number()                       { STANDARD_CONSTRUCTOR() }

   double asDouble() const                 { return getValue(); }
   int asInt() const                       { return getIntValue(); }

private:
   virtual double getValue() const = 0;
   virtual int getIntValue() const = 0;
};

}
}

#endif
