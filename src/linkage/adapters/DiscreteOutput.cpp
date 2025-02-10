
#include "mixr/linkage/adapters/DiscreteOutput.hpp"

#include "mixr/base/concepts/linkage/IIoData.hpp"
#include "mixr/base/concepts/linkage/IIoDevice.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(DiscreteOutput, "DiscreteOutput")
EMPTY_DELETEDATA(DiscreteOutput)

BEGIN_SLOTTABLE(DiscreteOutput)
    "do",         // 1) Discrete Output location (IIoData's DO channel)
    "port",       // 2) Device port number (default: 0)
    "channel",    // 3) Device channel (bit) number on the port
    "inverted",   // 4) Inverted bit flag (default: false)
    "value",      // 5) Initial value (default: false)
    "count"       // 6) Number of DIs to manage starting at 'di' and 'channel'
END_SLOTTABLE(DiscreteOutput)

BEGIN_SLOT_MAP(DiscreteOutput)
    ON_SLOT( 1, setSlotLocation, base::Integer)
    ON_SLOT( 2, setSlotPort,     base::Integer)
    ON_SLOT( 3, setSlotChannel,  base::Integer)
    ON_SLOT( 4, setSlotInverted, base::Boolean)
    ON_SLOT( 5, setSlotValue,    base::Boolean)
    ON_SLOT( 6, setSlotCount,    base::Integer)
END_SLOT_MAP()

DiscreteOutput::DiscreteOutput()
{
   STANDARD_CONSTRUCTOR()
}

void DiscreteOutput::copyData(const DiscreteOutput& org, const bool)
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

void DiscreteOutput::processOutputsImpl(const base::IIoData* const outData, base::IIoDevice* const device)
{
   if (device != nullptr && devEnb) {
      int chan = channel;
      int loc = location;
      int n = ((count >= 0) ? count : -count);

      for (int i = 0; i < n; i++) {

         // Get the bit from the cockpit output data handler
         if (outData != nullptr) {
            outData->getDiscreteOutput(loc, &value);
         }

         // Send the bit to the DO card
         bool value0 = value;
         if (invert) value0 = !value;
         device->setDiscreteOutput(value0, chan, port);

         chan++;
         if (count >= 0) loc++;
         else loc--;
      }
   }
}

// location: Output array index (location)
bool DiscreteOutput::setSlotLocation(const base::Integer* const msg)
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
bool DiscreteOutput::setSlotPort(const base::Integer* const msg)
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
bool DiscreteOutput::setSlotChannel(const base::Integer* const msg)
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
bool DiscreteOutput::setSlotInverted(const base::Boolean* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setInvertFlag( msg->asBool() );
   }
   return ok;
}

// value: Initial value (default: false)
bool DiscreteOutput::setSlotValue(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setValue( msg->asBool() );
   }
   return ok;
}

// count: number of discrete bits
bool DiscreteOutput::setSlotCount(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setCount( msg->asInt() );
   }
   return ok;
}

}
}
