
#include "mixr/linkage/generators/DiscreteInputFixed.hpp"

#include "mixr/base/numeric/Integer.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/Identifier.hpp"

#include <cmath>
#include <string>
#include <algorithm>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(DiscreteInputFixed, "DiscreteInputFixed")
EMPTY_DELETEDATA(DiscreteInputFixed)

BEGIN_SLOTTABLE(DiscreteInputFixed)
    "di",         // 1) Discrete channel index
    "signal",     // 2) Type identifier { on, off }
END_SLOTTABLE(DiscreteInputFixed)

BEGIN_SLOT_MAP(DiscreteInputFixed)
   ON_SLOT( 1, setSlotChannel, base::Integer)
   ON_SLOT( 2, setSlotSignal,  base::Identifier)
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

bool DiscreteInputFixed::setSlotChannel(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v = msg->asInt();
      if (v >= 0) {
         ok = setChannel(v);
      }
   }
   return ok;
}

bool DiscreteInputFixed::setSlotSignal(const base::Identifier* const x)
{
   bool ok{};
   if (x != nullptr) {

      std::string signalType(x->c_str());
      // convert to lowercase
      std::transform(signalType.begin(), signalType.end(), signalType.begin(), ::tolower);
      // set the type
      if (signalType == "on")         ok = setSignalType(Signal::ON);
      else if (signalType == "off")   ok = setSignalType(Signal::OFF);

      if (!ok && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "DiscreteInputFixed::setSlotSignal(): Invalid signal type: " << signalType;
         std::cerr << ", use: { on, off }" << std::endl;
      }

   }
   return ok;
}

}
}
