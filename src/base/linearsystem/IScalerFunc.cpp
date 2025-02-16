
#include "mixr/base/linearsystem/IScalerFunc.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/frequencies.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(IScalerFunc, "IScalerFunc")

BEGIN_SLOTTABLE(IScalerFunc)
    "rate",   //  1: Master update rate
    "x0",     //  2: Initial (previous) input value: X(0)
    "y0",     //  3: Initial (previous) output value: Y(0)
END_SLOTTABLE(IScalerFunc)

BEGIN_SLOT_MAP(IScalerFunc)
   ON_SLOT( 1, setSlotRate, IFrequency)
   ON_SLOT( 1, setSlotRate, Integer)
   ON_SLOT( 2, setSlotX0,   INumber)
   ON_SLOT( 3, setSlotY0,   INumber)
END_SLOT_MAP()

IScalerFunc::IScalerFunc()
{
   STANDARD_CONSTRUCTOR()
}

IScalerFunc::IScalerFunc(const unsigned int r): rate(r)
{
   STANDARD_CONSTRUCTOR()
}

void IScalerFunc::copyData(const IScalerFunc& org, const bool)
{
   BaseClass::copyData(org);

   rate = org.rate;
   x0 = org.x0;
   y0 = org.y0;

   // copy data array -- derived classes must have called allocateMemory() prior
   // to calling this copyData() function.
   allocateMemory( org.n );
   if (n > 0) {
      for (unsigned int i{}; i < n; i++) {
         px[i] = org.px[i];
         py[i] = org.py[i];
      }
   }
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void IScalerFunc::deleteData()
{
   // We clean up memory so that our derived classes don't need to.
   allocateMemory(0);
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool IScalerFunc::isValid() const
{
   return (px != nullptr && py != nullptr && n > 0 && BaseClass::isValid());
}

//------------------------------------------------------------------------------
// Initialize this control/filter
//------------------------------------------------------------------------------
void IScalerFunc::initialize()
{
}

//------------------------------------------------------------------------------
// Allocate memory arrays and free any old arrays
//------------------------------------------------------------------------------
void IScalerFunc::allocateMemory(const unsigned int n0)
{
   if (n0 != n) {
      // Free the old memory arrays
      if (n > 0) {
         delete[] px;  px = nullptr;
         delete[] py;  py = nullptr;
         n = 0;
      }
      // Allocate the new space
      if (n0 > 0) {
         px = new double[n0];
         py = new double[n0];
         n = n0;
      }
   }
}

//------------------------------------------------------------------------------
// Clear the memory arrays
//------------------------------------------------------------------------------
void IScalerFunc::clearMemory()
{
   for (unsigned int i{}; i < n; i++) {
      px[i] = 0;
      py[i] = 0;
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool IScalerFunc::setX0(const double v)
{
   x0 = v;
   if (px != nullptr) {
      for (unsigned int i{}; i < n; i++) {
         px[i] = x0;
      }
   }
   return true;
}

bool IScalerFunc::setY0(const double v)
{
   y0 = v;
   if (py != nullptr) {
      for (unsigned int i{}; i < n; i++) {
         py[i] = y0;
      }
   }
   return true;
}

bool IScalerFunc::setRate(const unsigned int v)
{
   bool ok {};
   if (v > 0) {
      rate = v;
      initialize();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool IScalerFunc::setSlotRate(const IFrequency* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {static_cast<int>( msg->getValueInHertz() + 0.5 )};
      if (v > 0) {
         setRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}

bool IScalerFunc::setSlotRate(const Integer* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {msg->asInt()};
      if (v > 0) {
         setRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}

bool IScalerFunc::setSlotX0(const INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setX0( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool IScalerFunc::setSlotY0(const INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setY0( msg->asDouble() );
      ok = true;
   }
   return ok;
}

}
}
