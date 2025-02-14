
#include "mixr/linkage/generators/AnalogInputFixed.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/concepts/linkage/IIoData.hpp"
#include "mixr/base/concepts/linkage/IIoDevice.hpp"

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(AnalogInputFixed, "AnalogInputFixed")
EMPTY_DELETEDATA(AnalogInputFixed)

BEGIN_SLOTTABLE(AnalogInputFixed)
    "ai",         // 1) Analog channel index
    "value",      // 2) Value to set
END_SLOTTABLE(AnalogInputFixed)

BEGIN_SLOT_MAP(AnalogInputFixed)
   ON_SLOT( 1, setSlotChannel, base::Integer)
   ON_SLOT( 2, setSlotValue,   base::INumber)
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

void AnalogInputFixed::processInputsImpl(const double dt, base::IIoData* const inData)
{
   // set the value in the input data buffer
   if (inData != nullptr) {
      inData->setAnalogInput(channel, value);
   }
}

bool AnalogInputFixed::setSlotChannel(const base::Integer* const msg)
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

bool AnalogInputFixed::setSlotValue(const base::INumber* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setValue(msg->asDouble());
   }
   return ok;
}

}
}
