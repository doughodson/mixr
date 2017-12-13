
#include "mixr/linkage/adapters/AnalogOutput.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/functors/Table1.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(AnalogOutput, "AnalogOutput")

BEGIN_SLOTTABLE(AnalogOutput)
    "ao",         // 1) Analog Output location (AbstractIoData AO's channel)
    "channel",    // 2) Device channel number
    "offset",     // 3) Offset value (default: 0.0)
    "gain",       // 4) Gain value   (default: 1.0)
    "table"       // 5) Shaping function table (default: none)
END_SLOTTABLE(AnalogOutput)

BEGIN_SLOT_MAP(AnalogOutput)
    ON_SLOT( 1, setSlotLocation, base::Number)
    ON_SLOT( 2, setSlotChannel,  base::Number)
    ON_SLOT( 3, setSlotOffset,   base::Number)
    ON_SLOT( 4, setSlotGain,     base::Number)
    ON_SLOT( 5, setSlotTable,    base::Table1)
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
   gain = org.gain;
   offset = org.offset;
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

void AnalogOutput::processOutputsImpl(const base::AbstractIoData* const outData, base::AbstractIoDevice* const device)
{
   double value{};

   // Get a value from the cockpit output handler
   if (outData != nullptr) {
      outData->getAnalogOutput(location, &value);
   }

   // Send the scaled data to the AO card
   if (device != nullptr) {
      double vout {};
      if (gain != 0) {
         vout = (value / gain ) + offset;
      }
      device->setAnalogOutput(vout, channel);
   }
}

// location: Output array index (location)
bool AnalogOutput::setSlotLocation(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {msg->getInt()};
      if (v >= 0) {
         ok = setLocation(v);
      }
   }
   return ok;
}

// channel: AI card's channel number
bool AnalogOutput::setSlotChannel(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      const int v {msg->getInt()};
      if (v >= 0) {
         ok = setChannel(v);
      }
   }
   return ok;
}

// offset: Offset value
bool AnalogOutput::setSlotOffset(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setOffset( msg->getFloat() );
   }
   return ok;
}

// gain: Gain value
bool AnalogOutput::setSlotGain(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setGain( msg->getFloat() );
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "AnalogOutput::setSlotGain(): ERROR -- gain can not be zero." << std::endl;
         }
      }
   }
   return ok;
}

}
}
