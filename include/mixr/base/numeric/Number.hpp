
#ifndef __mixr_base_numeric_Number_H__
#define __mixr_base_numeric_Number_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Number
//
// Description:  Abstract class for number-based objects (Float, Integer) and 
//               numerical operators (Add, Subtract, Multiply and Divide)
//------------------------------------------------------------------------------
class Number : public Object
{
   DECLARE_SUBCLASS(Number, Object)

public:
   explicit Number()                       { STANDARD_CONSTRUCTOR() }

   double asDouble() const                 { return getValue(); }

private:
   virtual double getValue() const = 0;
};

}
}

#endif
