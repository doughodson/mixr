
#include "mixr/linkage/adapters/AnalogOutput.hpp"

#include "mixr/base/concepts/linkage/IIoData.hpp"
#include "mixr/base/concepts/linkage/IIoDevice.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/relations/Table1.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(AnalogOutput, "AnalogOutput")

BEGIN_SLOTTABLE(AnalogOutput)
    "ao",         // 1) Analog Output location (AbstractIoData AO's channel)
    "channel",    // 2) Device channel number
    "offset",     // 3) Offset value (default: 0.0)
    "gain",       // 4) Gain value   (default: 1.0)
    "table",      // 5) Shaping function table (default: none)
    "value"       // 6) Initial value [ -1.0 ... 1.0 ] (default: 0.0)
END_SLOTTABLE(AnalogOutput)

BEGIN_SLOT_MAP(AnalogOutput)
    ON_SLOT( 1, setSlotLocation, base::Integer)
    ON_SLOT( 2, setSlotChannel,  base::Integer)
    ON_SLOT( 3, setSlotOffset,   base::INumber)
    ON_SLOT( 4, setSlotGain,     base::INumber)
    ON_SLOT( 5, setTable,        base::Table1)
    ON_SLOT( 6, setSlotValue,    base::INumber)
END_SLOT_MAP()

AnalogOutput::AnalogOutput()
{
   STANDARD_CONSTRUCTOR()
}

void AnalogOutput::copyData(const AnalogOutput& org, const bool)
{
   BaseClass::copyData(org);

   location = org.location;
   channel = org.channel;
   devEnb = org.devEnb;
   gain = org.gain;
   offset = org.offset;
   value = org.value;
   {
      const base::Table1* copy {};
      if (org.table != nullptr) {
         copy = org.table->clone();
      }
      setTable(copy);
      if (copy != nullptr) copy->unref();
   }
}

void AnalogOutput::deleteData()
{
   setTable(nullptr);
}

// table: Shaping function table
bool AnalogOutput::setTable(const base::Table1* const msg)
{
    bool ok {true};

    // Unref() the old (if any)
    if (table != nullptr) {
        table->unref();
    }

    // set our pointer to the new
    table = msg;

    // Check and ref() the new table (if any)
    if (table != nullptr) {
        if (table->isValid()) {
            table->ref();
        } else {
            if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "AnalogOutput::setTable(): invalid table!" << std::endl;
            }
            ok = false;
            table = nullptr;
        }
    }

    return ok;
}

void AnalogOutput::processOutputsImpl(const base::IIoData* const outData, base::IIoDevice* const device)
{
   if (device != nullptr && devEnb) {
      // Get a value from the cockpit output handler
      if (outData != nullptr) {
         outData->getAnalogOutput(location, &value);
      }

      // Send the scaled data to the AO card
      double vout {convert(value)};
      device->setAnalogOutput(vout, channel);
   }
}

//------------------------------------------------------------------------------
// convert the value, as needed
//------------------------------------------------------------------------------
double AnalogOutput::convert(const double vin)
{
   // Offset & Gain
   double v2{vin + offset};
   if (gain != 0) {
      v2 = (value / gain ) + offset;
   }

   // Shaping function
   double v3{v2};
   if (table != nullptr) v3 = table->lfi(v2);

   // return final value
   return v3;
}

// location: Output array index (location)
bool AnalogOutput::setSlotLocation(const base::Integer* const msg)
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

// channel: AI card's channel number
bool AnalogOutput::setSlotChannel(const base::Integer* const msg)
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

// offset: Offset value
bool AnalogOutput::setSlotOffset(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setOffset( msg->asDouble() );
   }
   return ok;
}

// gain: Gain value
bool AnalogOutput::setSlotGain(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setGain( msg->asDouble() );
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "AnalogOutput::setSlotGain(): ERROR -- gain can not be zero." << std::endl;
         }
      }
   }
   return ok;
}

// value: Initial value (default: 0)
bool AnalogOutput::setSlotValue(const base::INumber* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setValue( msg->asDouble() );
   }
   return ok;
}

}
}
