
#include "mixr/base/linearsystem/SaH.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/units/frequencies.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SaH, "SaH")
EMPTY_DELETEDATA(SaH)

BEGIN_SLOTTABLE(SaH)
    "sampleRate",   //  1: Sample update rate
END_SLOTTABLE(SaH)

BEGIN_SLOT_MAP(SaH)
   ON_SLOT( 1, setSlotSampleRate, base::IFrequency)
   ON_SLOT( 1, setSlotSampleRate, base::Integer)
END_SLOT_MAP()

SaH::SaH()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

SaH::SaH(const unsigned int r, const unsigned int sr) : ScalerFunc(r)
{
   STANDARD_CONSTRUCTOR()

   initData();
   sampleRate = sr;

   initialize();
}

void SaH::initData()
{
   allocateMemory(ORDER);
   clearMemory();
}

void SaH::copyData(const SaH& org, const bool cc)
{
   if (cc) {
      // allocate memory before calling BaseClass::copyData()
      allocateMemory(ORDER);
   }

   BaseClass::copyData(org);

   sampleRate = org.sampleRate;
   time = org.time;
   stime = org.stime;

   initialize();
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid List
//------------------------------------------------------------------------------
bool SaH::isValid() const
{
   // Valid if we have a master rate and a sample rate
   bool valid {(n == ORDER && rate > 0 && sampleRate > 0)};

   return valid && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// g() is one iteration of the difference equation.
//------------------------------------------------------------------------------
double SaH::g(const double xn)
{
   if (isValid()) {

      // Current input
      px[0] = xn;

      // Update sample timer
      time += 1.0f/rate;

      // Take a sample
      if (time >= stime) {
         py[0] = px[0];
         time = 0;
      }

      return py[0];
   } else {
      // If invalid transfer function, just return the input value
      return xn;
   }
}

//------------------------------------------------------------------------------
// initialize the system
//------------------------------------------------------------------------------
void SaH::initialize()
{
   BaseClass::initialize();

   if (isValid()) {
      // Init the past values
      px[0] = x0;
      py[0] = y0;

      // Compute sample time
      stime = 1.0f/sampleRate;

      // Age of the sample data: set beyond max to force a sample at first call to g()
      time = stime + 1.0f;
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool SaH::setSampleRate(const unsigned int v)
{
   sampleRate = v;
   initialize();
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool SaH::setSlotSampleRate(const base::IFrequency* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {static_cast<int>( msg->getValueInHertz() + 0.5 )};
      if (v > 0) {
         setSampleRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}

bool SaH::setSlotSampleRate(const base::Integer* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {msg->asInt()};
      if (v > 0) {
         setSampleRate( static_cast<unsigned int>(v) );
         ok = true;
      }
   }
   return ok;
}

}
}
