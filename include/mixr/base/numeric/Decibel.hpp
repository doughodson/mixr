
#ifndef __mixr_base_numeric_Decibel_HPP__
#define __mixr_base_numeric_Decibel_HPP__

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Decibel
// Description: The decibel (dB) is a logarithmic unit of measurement that expreses
//              the magnitude of a physical quantity (usually power or intensity)
//              relative to a specified of implied reference level.  Since it
//              expresses a ratio of two quantities of the same unit, it is a
//              dimensionless unit. This class conveys that the value it represents
//              is a scaled number.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: dB
// Slots:
//     value  <Number>  ! Sets the value of this number (default: 0.0)
//------------------------------------------------------------------------------
class Decibel final: public Number
{
   DECLARE_SUBCLASS(Decibel, Number)

public:
   explicit Decibel();
   explicit Decibel(const double);

   void setValuedB(const double);
   double asdB() const                 { return db; }

   void setValue(const double);

private:
   double getValue() const final       { return value; }
   double value{};

   double db{};      // value in dBs

private:
   // slot table helper methods
   bool setSlotValue(const Number* const);
};

}
}

#endif
