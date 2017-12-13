
#include "mixr/linkage/generators/DiscreteInputFixed.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/String.hpp"

#include <cmath>
#include <string>
#include <algorithm>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(DiscreteInputFixed, "DiscreteInputFixed")
EMPTY_DELETEDATA(DiscreteInputFixed)

BEGIN_SLOTTABLE(DiscreteInputFixed)
    "di",         // 1) Discrete channel index
    "signal",     // 2) Signal type { ON, OFF }
END_SLOTTABLE(DiscreteInputFixed)

BEGIN_SLOT_MAP(DiscreteInputFixed)
   ON_SLOT( 1, setSlotChannel,   base::Number)
   ON_SLOT( 2, setSlotSignal,    base::String)
END_SLOT_MAP()

DiscreteInputFixed::DiscreteInputFixed()
{
   STANDARD_CONSTRUCTOR()
}

void DiscreteInputFixed::copyData(const DiscreteInputFixed& org, const bool)
{
   BaseClass::copyData(org);
   signal = org.signal;
}

void DiscreteInputFixed::processInputsImpl(const double dt, base::AbstractIoData* const inData)
{
   // Send the value to the input data buffer
   if (inData != nullptr) {
      switch (signal) {
         case Signal::ON  : inData->setDiscreteInput(channel, true);  break;
         case Signal::OFF : inData->setDiscreteInput(channel, false); break;
      }
   }
}

//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------

bool DiscreteInputFixed::setSlotChannel(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v = msg->getInt();
      if (v >= 0) {
         ok = setChannel(v);
      }
   }
   return ok;
}

// signal: Signal type { ON, OFF }
bool DiscreteInputFixed::setSlotSignal(const base::String* const msg)
{
   bool ok{};
   if (msg != nullptr) {

      std::string signalType(msg->getString());
      // convert to lowercase
      std::transform(signalType.begin(), signalType.end(), signalType.begin(), ::tolower);
      // set the type
      if (signalType == "on")         ok = setSignalType(Signal::ON);
      else if (signalType == "off")   ok = setSignalType(Signal::OFF);

      if (!ok && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "DiscreteSignalGen::setSlotSignal(): Invalid signal type: " << signalType;
         std::cerr << ", use: { ON, OFF }" << std::endl;
      }

   }
   return ok;
}

}
}
