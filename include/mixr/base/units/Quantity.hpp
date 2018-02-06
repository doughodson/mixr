
#ifndef __mixr_base_Quantity_H__
#define __mixr_base_Quantity_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Quantity
// Description: Abstract base class for all units.  Provides storage for a
//              double precision value and a slot method to set it.
//
// Factory name: AbstractQuantity
//------------------------------------------------------------------------------
class Quantity : public Object
{
   DECLARE_SUBCLASS(Quantity, Object)

public:
   explicit Quantity();

protected:
   void setValue(const double x)     { value = x;    }
   double getValue() const           { return value; }

private:
   double value{};  // value in terms of base unit
   virtual double convertToBaseUnit(const double) =0;

private:
   // slot table helper methods
   bool setSlotValue(const Number* const);
};

}
}

#endif

