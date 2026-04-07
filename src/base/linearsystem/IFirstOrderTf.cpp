
#include "mixr/base/linearsystem/IFirstOrderTf.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(IFirstOrderTf, "IFirstOrderTf")
EMPTY_SLOTTABLE(IFirstOrderTf)
EMPTY_DELETEDATA(IFirstOrderTf)

IFirstOrderTf::IFirstOrderTf()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

IFirstOrderTf::IFirstOrderTf(const unsigned int ir,
         const double in1, const double in2,
         const double id1, const double id2
         ) : IDiffEquation(ir)

{
   STANDARD_CONSTRUCTOR()

   initData();

   n1 = in1;
   n2 = in2;
   d1 = id1;
   d2 = id2;

   initialize();
}

void IFirstOrderTf::initData()
{
   allocateMemory(ORDER);
   clearMemory();

   initialize();
}

void IFirstOrderTf::copyData(const IFirstOrderTf& org, const bool cc)
{
   if (cc) {
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   n1 = org.n1;
   n2 = org.n2;
   d1 = org.d1;
   d2 = org.d2;

   initialize();
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool IFirstOrderTf::isValid() const
{
   unsigned int orderN {};
   if (n1 != 0)
      orderN = 1;

   unsigned int orderD {};
   if (d1 != 0)
      orderD = 1;

   // Valid if we have a update rate and a value for the denominator, and the
   // order of the denominator is greater than or equal the numerator.
   bool valid {(
         n == ORDER &&
         rate > 0 &&
         (d1 != 0 || d2 != 0) &&
         (orderD >= orderN)
      )};

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void IFirstOrderTf::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      const double T {1.0 / static_cast<double>(rate)};
      pa[0] = 0;
      pa[1] = -(T*d2 - 2.0f*d1)/(T*d2 + 2.0f*d1);
      pb[0] =  (T*n2 + 2.0f*n1)/(T*d2 + 2.0f*d1);
      pb[1] =  (T*n2 - 2.0f*n1)/(T*d2 + 2.0f*d1);
      for (unsigned int k = 0; k < n; k++) {
         px[k] = x0;
         py[k] = y0;
      }

      if (isMessageEnabled(MSG_DEBUG)) {
         std::cout << "a[1] = " << pa[1] << std::endl;
         std::cout << "b[0] = " << pb[0] << std::endl;
         std::cout << "b[1] = " << pb[1] << std::endl;
      }
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool IFirstOrderTf::setN1(const double v)
{
   n1 = v;
   initialize();
   return true;
}

bool IFirstOrderTf::setN2(const double v)
{
   n2 = v;
   initialize();
   return true;
}

bool IFirstOrderTf::setD1(const double v)
{
   d1 = v;
   initialize();
   return true;
}

bool IFirstOrderTf::setD2(const double v)
{
   d2 = v;
   initialize();
   return true;
}

}
}
