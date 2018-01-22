
#ifndef __mixr_base_numeric_Number_H__
#define __mixr_base_numeric_Number_H__

#include "mixr/base/Object.hpp"
#include <cstdint>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Number
//
// Description:  Base class for several number-based objects (Float, Integer), 
//               numerical operators (Add, Subtract, Multiply and Divide),
//               and various units (see base/units/*.hpp)
//
// Slots:
//     value  <Number>  ! Sets the value of this number. (default: 0.0)
//
//
// Public methods:
//
//    Number(const double value)
//       Special constructor that initializes the number to 'value'
//
//    double getReal()
//    double getDouble()
//    float getFloat()
//    int getInt()
//       Return the value of the Number object as a double, double,
//       float, or integer, respectively, regardless of the derived
//       class type of the object.
//
//------------------------------------------------------------------------------
class Number : public Object
{
   DECLARE_SUBCLASS(Number, Object)

public:
   Number()                         { STANDARD_CONSTRUCTOR() }
   Number(const int value)          { val = static_cast<double>(value); STANDARD_CONSTRUCTOR() }
   Number(const double value)       { val = value; STANDARD_CONSTRUCTOR() }
   Number(const float value)        { val = static_cast<double>(value); STANDARD_CONSTRUCTOR() }

   double getReal() const           { return val; }
   double getDouble() const         { return val; }
   float getFloat() const           { return static_cast<float>(val); }
   //int getInt() const               { return static_cast<int>(val); }

   virtual void setValue(const double x)   { val = x; }

protected:
   double val{};   // value of this object

private:
   // slot table helper methods
   virtual bool setSlotValue(const Number* const);
};

}
}

#endif
