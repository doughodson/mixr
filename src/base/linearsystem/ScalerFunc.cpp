
#include "mixr/base/linearsystem/ScalerFunc.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/frequencies.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(ScalerFunc, "ScalerFunc")

BEGIN_SLOTTABLE(ScalerFunc)
    "rate",   //  1: Master update rate
    "x0",     //  2: Initial (previous) input value: X(0)
    "y0",     //  3: Initial (previous) output value: Y(0)
END_SLOTTABLE(ScalerFunc)

BEGIN_SLOT_MAP(ScalerFunc)
   ON_SLOT( 1, setSlotRate, IFrequency)
   ON_SLOT( 1, setSlotRate, Integer)
   ON_SLOT( 2, setSlotX0,   Number)
   ON_SLOT( 3, setSlotY0,   Number)
END_SLOT_MAP()

ScalerFunc::ScalerFunc()
{
   STANDARD_CONSTRUCTOR()
}

ScalerFunc::ScalerFunc(const unsigned int r): rate(r)
{
   STANDARD_CONSTRUCTOR()
}

void ScalerFunc::copyData(const ScalerFunc& org, const bool)
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
void ScalerFunc::deleteData()
{
   // We clean up memory so that our derived classes don't need to.
   allocateMemory(0);
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool ScalerFunc::isValid() const
{
   return (px != nullptr && py != nullptr && n > 0 && BaseClass::isValid());
}

//------------------------------------------------------------------------------
// Initialize this control/filter
//------------------------------------------------------------------------------
void ScalerFunc::initialize()
{
}

//------------------------------------------------------------------------------
// Allocate memory arrays and free any old arrays
//------------------------------------------------------------------------------
void ScalerFunc::allocateMemory(const unsigned int n0)
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
void ScalerFunc::clearMemory()
{
   for (unsigned int i{}; i < n; i++) {
      px[i] = 0;
      py[i] = 0;
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool ScalerFunc::setX0(const double v)
{
   x0 = v;
   if (px != nullptr) {
      for (unsigned int i{}; i < n; i++) {
         px[i] = x0;
      }
   }
   return true;
}

bool ScalerFunc::setY0(const double v)
{
   y0 = v;
   if (py != nullptr) {
      for (unsigned int i{}; i < n; i++) {
         py[i] = y0;
      }
   }
   return true;
}

bool ScalerFunc::setRate(const unsigned int v)
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
bool ScalerFunc::setSlotRate(const IFrequency* const msg)
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

bool ScalerFunc::setSlotRate(const Integer* const msg)
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

bool ScalerFunc::setSlotX0(const Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setX0( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool ScalerFunc::setSlotY0(const Number* const msg)
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
