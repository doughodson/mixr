
#ifndef __mixr_linkage_AnalogInput_HPP__
#define __mixr_linkage_AnalogInput_HPP__

#include "mixr/linkage/adapters/IAdapter.hpp"

namespace mixr {
namespace base { class IIoData; class IIoDevice; class Integer; class Number; class Table1; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: AnalogInput
//
// Description: Manages an analog input (AI).
//
// AI processing:
//
//    1) Deadband; The AI value is clamped to zero(0) if it's less
//       than +deadband and greater than -deadband.  There is no clamping if
//       deadband is zero (the default)
//
//          V1 = clamp(AI, deadband)
//
//
//    2) Offset and Gain: the input value (after deadband clamping) can
//       be offset and then scaled by gain.  Defaults are zero offset
//       and unity gain.
//
//          V2 = (V1 - offset) * gain
//
//
//    3) Optional shaping function: the input value (after gain and offset)
//       can be shaped using an 1-D LFI table.
//
//          V3 = f(V2)
//
// Factory name: AnalogInput
// Slots:
//      ai           <Integer>   ! Analog Input location (IoData's AI channel)
//      channel      <Integer>   ! Device channel number
//      deadband     <Number>    ! Deadband: [ 0 .. 1 ] (default: 0.0)
//      offset       <Number>    ! Offset value (default: 0.0)
//      gain         <Number>    ! Gain value   (default: 1.0)
//      table        <Table1>    ! Shaping function table (default: none)
//      value        <Number>    ! Initial value [ -1.0 ... 1.0 ] (default: 0.0)
//------------------------------------------------------------------------------
class AnalogInput final: public IAdapter
{
   DECLARE_SUBCLASS(AnalogInput, IAdapter)

public:
   AnalogInput();

   int getLocation() const                             { return location;   }
   int getChannel() const                              { return channel;    }
   double getDeadband() const                          { return deadband;   }
   double getOffset() const                            { return offset;     }
   double getGain() const                              { return gain;       }
   double getValue() const                             { return value;      }
   const base::Table1* getTable() const                { return table;      }

   bool setLocation(const int x)                       { location = x;   return true; }
   bool setChannel(const int x)                        { channel = x;    devEnb = true;  return true; }
   bool setDeadband(const double x)                    { deadband = x;   return true; }
   bool setOffset(const double x)                      { offset = x;     return true; }
   bool setGain(const double x)                        { gain = x;       return true; }
   bool setValue(const double x)                       { value = x;     return true; }
   bool setTable(const base::Table1* const);

private:
   void processInputsImpl(const base::IIoDevice* const device, base::IIoData* const inData) final;
   void processOutputsImpl(const base::IIoData* const outData, base::IIoDevice* const device) final   {}

   int location{};              // AbstractIoData analog input channel number
   int channel{};               // Analog channel number
   bool devEnb{};               // Device enabled
   double deadband{};           // Deadband value
   double offset{};             // Offset
   double gain{1.0};            // Gain
   const base::Table1* table{}; // Shaping table
   double value{0.0};           // Initial value

protected:
   virtual double convert(const double);

private:
   // slot table helper methods
   bool setSlotLocation(const base::Integer* const);
   bool setSlotChannel(const base::Integer* const);
   bool setSlotDeadband(const base::Number* const);
   bool setSlotOffset(const base::Number* const);
   bool setSlotGain(const base::Number* const);
   bool setSlotValue(const base::Number* const);
};

}
}

#endif
