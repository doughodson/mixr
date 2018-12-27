
#ifndef __mixr_base_numeric_Float_H__
#define __mixr_base_numeric_Float_H__

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Float
// Description: Class that stores an EDL defined (regex) float point value as a
//              C++ double
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Float
// Slots:
//     value  <Float>  ! Sets the value of this number (default: 0.0)
//------------------------------------------------------------------------------
class Float final: public Number
{
   DECLARE_SUBCLASS(Float, Number)

public:
   explicit Float()                                   { STANDARD_CONSTRUCTOR() }
   explicit Float(const double x) : value(x)          { STANDARD_CONSTRUCTOR() }
   explicit Float(const float x)  : value(x)          { STANDARD_CONSTRUCTOR() }

   void setValue(const double x)       { value = x; }

private:
   double getValue() const final       { return value; }
   double value{};

private:
   // slot table helper methods
   virtual bool setSlotValue(const Float* const);
};

}
}

#endif
