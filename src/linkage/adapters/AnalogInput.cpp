
#include "mixr/linkage/adapters/AnalogInput.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/functors/Table1.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(AnalogInput, "AnalogInput")

BEGIN_SLOTTABLE(AnalogInput)
    "ai",         // 1) Analog Input location (AbstractIoData's AI channel)
    "channel",    // 2) Device channel number
    "deadband",   // 3) Deadband: [ 0 .. 1 ] (default: 0.0)
    "offset",     // 4) Offset value (default: 0.0)
    "gain",       // 5) Gain value   (default: 1.0)
    "table"       // 6) Shaping function table (default: none)
END_SLOTTABLE(AnalogInput)

BEGIN_SLOT_MAP(AnalogInput)
    ON_SLOT( 1, setSlotLocation, base::Number)
    ON_SLOT( 2, setSlotChannel,  base::Number)
    ON_SLOT( 3, setSlotDeadband, base::Number)
    ON_SLOT( 4, setSlotOffset,   base::Number)
    ON_SLOT( 5, setSlotGain,     base::Number)
    ON_SLOT( 6, setTable,        base::Table1)
END_SLOT_MAP()

AnalogInput::AnalogInput()
{
   STANDARD_CONSTRUCTOR()
}

void AnalogInput::copyData(const AnalogInput& org, const bool)
{
   BaseClass::copyData(org);

   location = org.location;
   channel = org.channel;
   deadband = org.deadband;
   gain = org.gain;
   offset = org.offset;
   {
      const base::Table1* copy{};
      if (org.table != nullptr) {
         copy = org.table->clone();
      }
      setTable(copy);
      if (copy != nullptr) copy->unref();
   }
}

void AnalogInput::deleteData()
{
   setTable(nullptr);
}

// table: Shaping function table
bool AnalogInput::setTable(const base::Table1* const msg)
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
                std::cerr << "AnalogInput::setTable(): invalid table!" << std::endl;
            }
            ok = false;
            table = nullptr;
        }
    }

    return ok;
}

void AnalogInput::processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData)
{
   // Default is our initial value
   double vin{};

   // Get data from the AI card
   if (device != nullptr) {
      device->getAnalogInput(&vin, channel);
   }

   // process the input value, as needed
   double vout {convert(vin)};

   // Set the data to the input data handler
   if (inData != nullptr) {
      inData->setAnalogInput(location, vout);
   }
}

//------------------------------------------------------------------------------
// convert the value, as needed
//------------------------------------------------------------------------------
double AnalogInput::convert(const double vin)
{
   // Deadband
   double v1 {vin};
   if (deadband != 0 && vin < deadband && vin > -deadband) {
      v1 = 0;
   }

   // Offset & Gain
   const double v2 {(v1 - offset) * gain};

   // Shaping function
   double v3 {v2};
   if (table != nullptr) v3 = table->lfi(v2);

   // return final value
   return v3;
}

// ai: Analog Input location
bool AnalogInput::setSlotLocation(const base::Number* const msg)
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
bool AnalogInput::setSlotChannel(const base::Number* const msg)
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

// deadband: Deadband: [ 0 .. 1 ] (default: 0.0)
bool AnalogInput::setSlotDeadband(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setDeadband( msg->getReal() );
   }
   return ok;
}

// offset: Offset value
bool AnalogInput::setSlotOffset(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setOffset( msg->getReal() );
   }
   return ok;
}

// gain: Gain value
bool AnalogInput::setSlotGain(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      ok = setGain( msg->getReal() );
   }
   return ok;
}

}
}
