
#include "mixr/base/linearsystem/LowpassFilter.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/qty/frequencies.hpp"
#include "mixr/base/util/constants.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(LowpassFilter,"LowpassFilter")
EMPTY_COPYDATA(LowpassFilter)
EMPTY_DELETEDATA(LowpassFilter)

BEGIN_SLOTTABLE(LowpassFilter)
    "wc",      //  1: Cutoff Frequency
    "wc",      //  1: Filer time constant (radians/sec)
END_SLOTTABLE(LowpassFilter)

BEGIN_SLOT_MAP(LowpassFilter)
   ON_SLOT( 1, setSlotWc, base::IFrequency)
   ON_SLOT( 1, setSlotWc, base::INumber)
END_SLOT_MAP()

LowpassFilter::LowpassFilter()
{
   STANDARD_CONSTRUCTOR()
}

LowpassFilter::LowpassFilter(const unsigned int rate, const double w)
                        : FirstOrderTf(rate, 0.0f, w, 1.0f, w)
{
   STANDARD_CONSTRUCTOR()
   wc = w;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool LowpassFilter::setWc(const double v)
{
   wc = v;

   setN1(0.0f);
   setN2(wc);
   setD1(1.0f);
   setD2(wc);

   initialize();
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool LowpassFilter::setSlotWc(const base::IFrequency* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double hz = msg->getValueInHertz();
      double rps = static_cast<double>( hz * 2.0 * base::PI);
      if (rps > 0) {
         setWc( rps );
         ok = true;
      }
      else {
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LowpassFilter::setSlotWc() -- Wc must be greater than zero!" << std::endl;
      }
   }
   }
   return ok;
}

bool LowpassFilter::setSlotWc(const base::INumber* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double rps = msg->asDouble();
      if (rps > 0) {
         setWc( rps );
         ok = true;
      }
      else {
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LowpassFilter::setSlotWc() -- Wc must be greater than zero!" << std::endl;
         }
      }
   }
   return ok;
}

}
}
