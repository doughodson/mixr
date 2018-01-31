
#include "mixr/linkage/adapters/DiscreteInput.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(DiscreteInput, "DiscreteInput")
EMPTY_DELETEDATA(DiscreteInput)

BEGIN_SLOTTABLE(DiscreteInput)
    "di",         // 1) Discrete Input location (AbstractIoData's DI channel)
    "port",       // 2) Device port number (default: 0)
    "channel",    // 3) Device channel (bit) number on the port
    "inverted",   // 4) Inverted bit flag (default: false)
END_SLOTTABLE(DiscreteInput)

BEGIN_SLOT_MAP(DiscreteInput)
    ON_SLOT( 1, setSlotLocation, base::Integer)
    ON_SLOT( 2, setSlotPort,     base::Integer)
    ON_SLOT( 3, setSlotChannel,  base::Integer)
    ON_SLOT( 4, setSlotInverted, base::Boolean)
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
   invert = org.invert;
}

void DiscreteInput::processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData)
{
   if (inData != nullptr) {
      bool value{};

      // Get the bit from the device card
      if (device != nullptr) {
         device->getDiscreteInput(&value, channel, port);
      }

      // Set the bit to the cockpit input handler
      bool value0 {value};
      if (invert)
         value0 = !value;
      inData->setDiscreteInput(location, value0);
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

}
}
