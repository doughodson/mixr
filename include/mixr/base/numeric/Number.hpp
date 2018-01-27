
#ifndef __mixr_base_numeric_Number_H__
#define __mixr_base_numeric_Number_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Number
//
// Description:  Base class for number-based objects (Float, Integer) and 
//               numerical operators (Add, Subtract, Multiply and Divide)
//
// Slots:
//     value  <Number>  ! Sets the value of this number (default: 0.0)
//------------------------------------------------------------------------------
class Number : public Object
{
   DECLARE_SUBCLASS(Number, Object)

public:
   Number()                                { STANDARD_CONSTRUCTOR() }
   explicit Number(const double x)         { val = x; STANDARD_CONSTRUCTOR() }

//   explicit operator double() const      { return val; }

   virtual void setValue(const double x)   { val = x; }
   double to_double() const                { return val; }

   // deprecated interface
//   double getReal() const           { return val; }
//   double getDouble() const         { return val; }
//   float getFloat() const           { return static_cast<float>(val); }
//   int getInt() const               { return static_cast<int>(val); }

protected:
   double val{};

private:
   // slot table helper methods
   virtual bool setSlotValue(const Number* const);
};

}
}

#endif
