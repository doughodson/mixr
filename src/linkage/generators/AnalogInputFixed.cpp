
#include "mixr/linkage/generators/AnalogInputFixed.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(AnalogInputFixed, "AnalogInputFixed")
EMPTY_DELETEDATA(AnalogInputFixed)

BEGIN_SLOTTABLE(AnalogInputFixed)
    "ai",         // 1) Analog channel index
    "value",      // 2) Value to set
END_SLOTTABLE(AnalogInputFixed)

BEGIN_SLOT_MAP(AnalogInputFixed)
   ON_SLOT( 1, setSlotChannel,   base::Number)
   ON_SLOT( 2, setSlotValue,     base::Number)
END_SLOT_MAP()

AnalogInputFixed::AnalogInputFixed()
{
   STANDARD_CONSTRUCTOR()
}

void AnalogInputFixed::copyData(const AnalogInputFixed& org, const bool)
{
   BaseClass::copyData(org);
   channel = org.channel;
   value = org.value;
}

void AnalogInputFixed::processInputsImpl(const double dt, base::AbstractIoData* const inData)
{
   // set the value in the input data buffer
   if (inData != nullptr) {
      inData->setAnalogInput(channel, value);
   }
}

bool AnalogInputFixed::setSlotChannel(const base::Number* const msg)
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

bool AnalogInputFixed::setSlotValue(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setValue(msg->getDouble());
   }
   return ok;
}

}
}
