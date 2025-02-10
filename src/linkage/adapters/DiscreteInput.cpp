
#include "mixr/linkage/adapters/DiscreteInput.hpp"

#include "mixr/base/concepts/linkage/IIoData.hpp"
#include "mixr/base/concepts/linkage/IIoDevice.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(DiscreteInput, "DiscreteInput")
EMPTY_DELETEDATA(DiscreteInput)

BEGIN_SLOTTABLE(DiscreteInput)
    "di",         // 1) Discrete Input location (IIoData's DI channel)
    "port",       // 2) Device port number (default: 0)
    "channel",    // 3) Device channel (bit) number on the port
    "inverted",   // 4) Inverted bit flag (default: false)
    "value",      // 5) Initial value (default: false)
    "count"       // 6) Number of DIs to manage starting at 'di' and 'channel'
END_SLOTTABLE(DiscreteInput)

BEGIN_SLOT_MAP(DiscreteInput)
    ON_SLOT( 1, setSlotLocation, base::Integer)
    ON_SLOT( 2, setSlotPort,     base::Integer)
    ON_SLOT( 3, setSlotChannel,  base::Integer)
    ON_SLOT( 4, setSlotInverted, base::Boolean)
    ON_SLOT( 5, setSlotValue,    base::Boolean)
    ON_SLOT( 6, setSlotCount,    base::Integer)
END_SLOT_MAP()

DiscreteInput::DiscreteInput()
{
   STANDARD_CONSTRUCTOR()
}

void DiscreteInput::copyData(const DiscreteInput& org, const bool)
{
   BaseClass::copyData(org);

   location = org.location;
   port = org.port;
   channel = org.channel;
   devEnb = org.devEnb;
   invert = org.invert;
   value = org.value;
   count = org.count;
}

void DiscreteInput::processInputsImpl(const base::IIoDevice* const device, base::IIoData* const inData)
{
   if (inData != nullptr) {
      int chan = channel;
      int loc = location;
      int n = ((count >= 0) ? count : -count);
      if (n == 0) n = 1; // default to at least one

      for (int i = 0; i < n; i++) {

         // Get the bit from the DI card
         if (device != nullptr && devEnb) {
            device->getDiscreteInput(&value, chan, port);
         }

         // Send the bit to the cockpit input handler
         bool value0(value);
         if (invert) value0 = !value;
         inData->setDiscreteInput(loc,value0);

         chan++;
         if (count >= 0) loc++;
         else loc--;
      }
   }
}

// location: Input array index (location)
bool DiscreteInput::setSlotLocation(const base::Integer* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {msg->asInt()};
      if (v >= 0) {
         ok = setLocation(v);
      }
   }
   return ok;
}

// port: DiHandler's port number
bool DiscreteInput::setSlotPort(const base::Integer* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {msg->asInt()};
      if (v >= 0) {
         ok = setPort(v);
      }
   }
   return ok;
}

// channel: DiHandler's channel (bit) number on the port
bool DiscreteInput::setSlotChannel(const base::Integer* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {msg->asInt()};
      if (v >= 0) {
         ok = setChannel(v);
      }
   }
   return ok;
}

// invert: Inverted bit flag (default: false)
bool DiscreteInput::setSlotInverted(const base::Boolean* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setInvert( msg->asBool() );
   }
   return ok;
}

// value: Initial value (default: false)
bool DiscreteInput::setSlotValue(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setValue( msg->asBool() );
   }
   return ok;
}

// count: number of discrete bits
bool DiscreteInput::setSlotCount(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setCount( msg->asInt() );
   }
   return ok;
}

}
}

