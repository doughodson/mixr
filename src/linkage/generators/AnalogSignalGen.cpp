
#include "mixr/linkage/generators/AnalogSignalGen.hpp"

#include "mixr/base/numeric/Integer.hpp"

#include "mixr/base/concepts/linkage/IIoData.hpp"
#include "mixr/base/concepts/linkage/IIoDevice.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/qty/angles.hpp"
#include "mixr/base/qty/frequencies.hpp"

#include <cmath>
#include <string>
#include <algorithm>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(AnalogSignalGen, "AnalogSignalGen")
EMPTY_DELETEDATA(AnalogSignalGen)

BEGIN_SLOTTABLE(AnalogSignalGen)
    "ai",         // 1) Analog channel index
    "signal",     // 2) Type identifier { sine, cosine, square, saw }
    "frequency",  // 3) Signal frequency
    "phase",      // 4) Phase shift
END_SLOTTABLE(AnalogSignalGen)

BEGIN_SLOT_MAP(AnalogSignalGen)
   ON_SLOT( 1, setSlotChannel,   base::Integer)
   ON_SLOT( 2, setSlotSignal,    base::Identifier)
   ON_SLOT( 3, setSlotFrequency, base::IFrequency)
   ON_SLOT( 4, setSlotPhase,     base::IAngle)
END_SLOT_MAP()

AnalogSignalGen::AnalogSignalGen()
{
   STANDARD_CONSTRUCTOR()
}

void AnalogSignalGen::copyData(const AnalogSignalGen& org, const bool)
{
   BaseClass::copyData(org);

   signal = org.signal;
   phase = org.phase;
   freq = org.freq;
   time = org.time;
}

void AnalogSignalGen::reset()
{
   time = 0.0;
}

void AnalogSignalGen::processInputsImpl(const double dt, base::IIoData* const inData)
{
   const double value{calc(dt)};

   // Send the value to the input data buffer
   if (inData != nullptr) {
      inData->setAnalogInput(channel, value);
   }
}

//------------------------------------------------------------------------------
// Generate an input value (e.g., from the I/O device
//------------------------------------------------------------------------------
double AnalogSignalGen::calc(const double dt)
{
   // Default AI input value
   double value{};

   // Update time since reset
   time += dt;

   // Current angle (radians)
   const double alpha{(2.0 * base::PI * freq * time) + phase};

   // Local cycle (-PI to PI)
   const double beta{base::angle::aepcdRad(alpha)};

   switch (signal) {
      case Signal::SINE : {
         value = std::sin(beta);
         break;
      }
      case Signal::COSINE : {
         value = std::cos(beta);
         break;
      }
      case Signal::SQUARE : {
         if (beta > 0 && beta < base::PI) value = 1.0;
         else value = 0.0;
         break;
      }
      case Signal::SAW : {
         value = (beta/base::PI) + 1.0;
         break;
      }
   }

   if (value > 1.0) value = 1.0;
   else if (value < -1.0) value = -1.0;

   return value;
}

//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

bool AnalogSignalGen::setSlotChannel(const base::Integer* const x)
{
   bool ok{};
   if (x != nullptr) {
      const int v{x->asInt()};
      if (v >= 0) {
         ok = setChannel(v);
      }
   }
   return ok;
}

// signal: Signal type { SINE, COSINE, SQUARE, SAW }
bool AnalogSignalGen::setSlotSignal(const base::Identifier* const x)
{
   bool ok{};
   if (x != nullptr) {

      std::string signalType{x->c_str()};
      // convert to lowercase
      std::transform(signalType.begin(), signalType.end(), signalType.begin(), ::tolower);
      // set the type
      if (signalType == "sine")         ok = setSignalType(Signal::SINE);
      else if (signalType == "cosine")  ok = setSignalType(Signal::COSINE);
      else if (signalType == "square")  ok = setSignalType(Signal::SQUARE);
      else if (signalType == "saw")     ok = setSignalType(Signal::SAW);

      if (!ok && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "AnalogSignalGen::setSlotSignal(): Invalid signal type: " << signalType;
         std::cerr << ", use: { sine, cosine, square, saw }" << std::endl;
      }

   }
   return ok;
}

// frequency: frequency
bool AnalogSignalGen::setSlotFrequency(const base::IFrequency* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setFrequency(x->getValueInHertz());
   }
   return ok;
}

// phase: Phase shift
bool AnalogSignalGen::setSlotPhase(const base::IAngle* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setPhase(x->getValueInRadians());
   }
   return ok;
}

}
}
