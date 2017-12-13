
#include "mixr/linkage/adapters/Ai2DiSwitch.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/numeric/Number.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(Ai2DiSwitch, "Ai2DiSwitch")
EMPTY_DELETEDATA(Ai2DiSwitch)

BEGIN_SLOTTABLE(Ai2DiSwitch)
    "di",         // 1) Discrete Input location (AbstractIoData's DI channel)
    "channel",    // 2) Device's AI channel number
    "level",      // 3) Level to switch DI (default: 0)
    "inverted"    // 4) Inverted bit flag (default: false)
END_SLOTTABLE(Ai2DiSwitch)

BEGIN_SLOT_MAP(Ai2DiSwitch)
    ON_SLOT( 1, setSlotLocation, base::Number)
    ON_SLOT( 2, setSlotChannel,  base::Number)
    ON_SLOT( 3, setSlotLevel,    base::Number)
    ON_SLOT( 4, setSlotInverted, base::Number)
END_SLOT_MAP()

Ai2DiSwitch::Ai2DiSwitch()
{
   STANDARD_CONSTRUCTOR()
}

void Ai2DiSwitch::copyData(const Ai2DiSwitch& org, const bool)
{
   BaseClass::copyData(org);

   location = org.location;
   channel = org.channel;
   level = org.level;
   invert = org.invert;
}

void Ai2DiSwitch::processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData)
{
   // Default is our initial value
   double vin {};

   // Get data from the AI device
   if (device != nullptr) {
      device->getAnalogInput(&vin, channel);
   }

   // Set the bit to the cockpit input handler
   if (inData != nullptr) {
      bool flag {vin >= level};
      if (invert) flag = !flag;
      inData->setDiscreteInput(location,flag);
   }
}

// location: Input array index (location)
bool Ai2DiSwitch::setSlotLocation(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v {msg->getInt()};
      if (v >= 0) {
         ok = setLocation(v);
      }
   }
   return ok;
}

// channel: Device's AI channel number
bool Ai2DiSwitch::setSlotChannel(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v {msg->getInt()};
      if (v >= 0) {
         ok = setChannel(v);
      }
   }
   return ok;
}

// level: Level to switch DI (default: 0)
bool Ai2DiSwitch::setSlotLevel(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setLevel( msg->getReal() );
   }
   return ok;
}

// invert: Inverted bit flag (default: false)
bool Ai2DiSwitch::setSlotInverted(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setInvertFlag( msg->getBoolean() );
   }
   return ok;
}

}
}
