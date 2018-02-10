
#ifndef __mixr_base_numeric_Float_H__
#define __mixr_base_numeric_Float_H__

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Float
// Description: Class that stores an EDL defined (regex) float point value as a C++ double
//------------------------------------------------------------------------------
// Factory name: Float
//------------------------------------------------------------------------------
// Slots:
//     value  <Float>  ! Sets the value of this number (default: 0.0)
//------------------------------------------------------------------------------
class Float final: public Number
{
   DECLARE_SUBCLASS(Float, Number)

public:
   explicit Float()                                         { STANDARD_CONSTRUCTOR() }
   explicit Float(const double value) : val(value)          { STANDARD_CONSTRUCTOR() }
   explicit Float(const float value)  : val(value)          { STANDARD_CONSTRUCTOR() }

   void setValue(const double x)       { val = x; }

private:
   double getValue() const final       { return val; }
   double val{};

private:
   // slot table helper methods
   virtual bool setSlotValue(const Float* const);
};

}
}

#endif
