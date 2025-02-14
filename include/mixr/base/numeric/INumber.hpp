
#ifndef __mixr_base_numeric_Number_HPP__
#define __mixr_base_numeric_Number_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: INumber
// Description:  Interface for number-based objects (Float, Integer) and
//               numerical operators (Add, Subtract, Multiply and Divide)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: INumber
// Slots: none
//------------------------------------------------------------------------------
class INumber : public Object
{
   DECLARE_SUBCLASS(INumber, Object)

public:
   explicit INumber()                       { STANDARD_CONSTRUCTOR() }

   double asDouble() const                 { return getValue(); }
   int asInt() const                       { return getIntValue(); }

private:
   virtual double getValue() const = 0;
   virtual int getIntValue() const = 0;
};

}
}

#endif
