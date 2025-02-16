
#include "mixr/base/linearsystem/LimitFunc.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(LimitFunc, "LimitFunc")
EMPTY_SLOTTABLE(LimitFunc)
EMPTY_DELETEDATA(LimitFunc)

LimitFunc::LimitFunc()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

LimitFunc::LimitFunc(const double ll, const double uu) : IScalerFunc()
{
   STANDARD_CONSTRUCTOR()

   initData();

   lower = ll;
   upper = uu;

   initialize();
}

void LimitFunc::initData()
{
   allocateMemory(ORDER);
   clearMemory();
}

void LimitFunc::copyData(const LimitFunc& org, const bool cc)
{
   if (cc) {
      // allocate memory before calling BaseClass::copyData()
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   lower = org.lower;
   upper = org.upper;

   initialize();
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool LimitFunc::isValid() const
{
   // Valid if we have a master rate and a sample rate
   bool valid {(n == ORDER && lower <= upper)};

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// g() is one iteration of the difference equation.
//------------------------------------------------------------------------------
double LimitFunc::g(const double xn)
{
   if (isValid()) {

      // Current input
      px[0] = xn;

      // LimitFunc the input
      double tmp {xn};

      if (tmp > upper) tmp = upper;
      else if (tmp < lower) tmp = lower;

      py[0] = tmp;

      return py[0];
   } else {
      // If invalid transfer function, just return the input value
      return xn;
   }
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void LimitFunc::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      // Init the past values
      px[0] = x0;
      py[0] = y0;
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool LimitFunc::setLowerLimit(const double v)
{
   lower = v;
   initialize();
   return true;
}

bool LimitFunc::setUpperLimit(const double v)
{
   upper = v;
   initialize();
   return true;
}

}
}
