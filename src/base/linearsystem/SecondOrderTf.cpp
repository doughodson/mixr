
#include "mixr/base/linearsystem/SecondOrderTf.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SecondOrderTf, "SecondOrderTf")
EMPTY_SLOTTABLE(SecondOrderTf)

SecondOrderTf::SecondOrderTf()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

SecondOrderTf::SecondOrderTf(const unsigned int ir,
         const double in1, const double in2, const double in3,
         const double id1, const double id2, const double id3
         ) : IDiffEquation(ir)

{
   STANDARD_CONSTRUCTOR()

   initData();

   n1 = in1;
   n2 = in2;
   n3 = in3;
   d1 = id1;
   d2 = id2;
   d3 = id3;

   initialize();
}

void SecondOrderTf::initData()
{
   allocateMemory(ORDER);
   clearMemory();
}

void SecondOrderTf::copyData(const SecondOrderTf& org, const bool cc)
{
   if (cc) {
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   n1 = org.n1;
   n2 = org.n2;
   n3 = org.n3;
   d1 = org.d1;
   d2 = org.d2;
   d3 = org.d3;

   initialize();
}

void SecondOrderTf::deleteData()
{
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool SecondOrderTf::isValid() const
{
   unsigned int orderN {};
   if (n1 != 0)
      orderN = 2;
   else if (n2 != 0)
      orderN = 1;

   unsigned int orderD {};
   if (d1 != 0)
      orderD = 2;
   else if (d2 != 0)
      orderD = 1;

   // Valid if we have a update rate and a value for the denominator, and the
   // order of the denominator is greater than or equal the numerator.
   bool valid = {(n == ORDER && rate > 0 && (d1 != 0 || d2 != 0 || d3 != 0) && (orderD >= orderN))};

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void SecondOrderTf::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      // ##### IN-WORK #####
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool SecondOrderTf::setN1(const double v)
{
   n1 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setN2(const double v)
{
   n2 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setN3(const double v)
{
   n3 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setD1(const double v)
{
   d1 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setD2(const double v)
{
   d2 = v;
   initialize();
   return true;
}

bool SecondOrderTf::setD3(const double v)
{
   d3 = v;
   initialize();
   return true;
}

}
}
