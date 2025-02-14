
#include "mixr/base/linearsystem/LagFilter.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/times.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(LagFilter, "LagFilter")
EMPTY_DELETEDATA(LagFilter)

BEGIN_SLOTTABLE(LagFilter)
    "tau",      //  1: Filer time constant (Time or sec)
END_SLOTTABLE(LagFilter)

BEGIN_SLOT_MAP(LagFilter)
   ON_SLOT( 1, setSlotTau, base::ITime)
   ON_SLOT( 1, setSlotTau, base::INumber)
END_SLOT_MAP()

LagFilter::LagFilter()
{
   STANDARD_CONSTRUCTOR()
}

LagFilter::LagFilter(const unsigned int rate, const double t)
                        : FirstOrderTf(rate, 0.0f, 1.0f, t, 1.0f)
{
   STANDARD_CONSTRUCTOR()

   tau = t;
}

// Copy data function
void LagFilter::copyData(const LagFilter& org, const bool)
{
    BaseClass::copyData(org);

    tau = org.tau;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool LagFilter::setTau(const double v)
{
   tau = v;

   setN1(0.0f);
   setN2(1.0f);
   setD1(tau);
   setD2(1.0f);

   initialize();
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool LagFilter::setSlotTau(const base::ITime* const x)
{
   bool ok{};
   if (x != nullptr) {
      const double tsec{x->getValueInSeconds()};
      if (tsec > 0) {
         setTau( tsec );
         ok = true;
      } else {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LagFilter::setSlotTau() -- tau must be greater than zero!" << std::endl;
         }
      }
   }
   return ok;
}

bool LagFilter::setSlotTau(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const double tsec {msg->asDouble()};
      if (tsec > 0) {
         setTau( tsec );
         ok = true;
      } else {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LagFilter::setSlotTau() -- tau must be greater than zero!" << std::endl;
         }
      }
   }
   return ok;
}

}
}
