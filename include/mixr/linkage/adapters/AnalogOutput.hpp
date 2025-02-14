
#ifndef __mixr_linkage_AnalogOutput_HPP__
#define __mixr_linkage_AnalogOutput_HPP__

#include "mixr/linkage/adapters/IAdapter.hpp"

namespace mixr {
namespace base { class IIoData; class IIoDevice; class Integer; class INumber; class Table1; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: AnalogOutput
//
// Description: Manages an analog output (AO).
//
// AO processing:
//
//    1) Offset and Gain: Defaults are zero offset and unity gain.
//
//          V1 = (value / gain ) + offset
//
//
//    2) Optional shaping function: the output value (after gain and offset)
//       can be shaped using an 1-D LFI table.
//
//          Vout = f(V1)
//
// Slots:
//      ao           <Integer>   ! Analog Output location (IoData AO's channel)
//      channel      <Integer>   ! Device channel number
//      offset       <INumber>   ! Offset value (default: 0.0)
//      gain         <INumber>   ! Gain value   (default: 1.0)
//      table        <Table1>    ! Shaping function table (default: none)
//      value        <INumber>   ! Initial value [ -1.0 ... 1.0 ] (default: 0.0)
//
//------------------------------------------------------------------------------
class AnalogOutput final: public IAdapter
{
   DECLARE_SUBCLASS(AnalogOutput, IAdapter)

public:
   AnalogOutput();


   int getLocation() const                             { return location;   }
   int getChannel() const                              { return channel;    }
   double getOffset() const                            { return offset;     }
   double getGain() const                              { return gain;       }
   double getValue() const                             { return value;      }
   const base::Table1* getTable() const                { return table;      }

   bool setLocation(const int x)                       { location = x;  return true; }
   bool setChannel(const int x)                        { channel = x;   devEnb = true;  return true; }
   bool setOffset(const double x)                      { offset = x;    return true; }
   bool setGain(const double x)                        { gain = x;      return true; }
   bool setValue(const double x)                       { value = x;     return true; }
   bool setTable(const base::Table1* const);

private:
   void processInputsImpl(const base::IIoDevice* const device, base::IIoData* const inData) final     {}
   void processOutputsImpl(const base::IIoData* const outData, base::IIoDevice* const device) final;

   int location{};               // AbstractIoData analog output channel number
   int channel{};                // Analog channel number
   bool devEnb{};                // Device enabled
   double offset{};              // Offset:  value = gain * (vin - offset)
   double gain{1.0};             // Gain:    value = gain * (vin - offset)
   const base::Table1* table{};  // Shaping table
   double value{0.0};            // Initial value

protected:
   virtual double convert(const double);

private:
   // slot table helper methods
   bool setSlotLocation(const base::Integer* const);
   bool setSlotChannel(const base::Integer* const);
   bool setSlotOffset(const base::INumber* const);
   bool setSlotGain(const base::INumber* const);
   bool setSlotValue(const base::INumber* const);
};

}
}

#endif
