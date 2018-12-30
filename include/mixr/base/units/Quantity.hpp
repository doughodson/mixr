
#ifndef __mixr_base_Quantity_HPP__
#define __mixr_base_Quantity_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Quantity
// Description: Abstract base class for all quantities (i.e., a numerical value
//              with an associated unit).  The numerical value is stored as a
//              double precision value and a slot method is available set it.
//              The class hierarchy is used to organize a taxonomy of different
//              kinds of quanities (e.g., Angle, Area, Energy, Power, etc.).
//
//              These classes are used to abstract away specific configuration
//              input values (for a given unit) from their use within a specific
//              model.
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
