
#ifndef __mixr_linkage_AnalogOutput_H__
#define __mixr_linkage_AnalogOutput_H__

#include "mixr/linkage/adapters/AbstractAdapter.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; class Number; class Table1; }
namespace linkage {

//------------------------------------------------------------------------------
// Class:  AnalogOutput
//
// Description:  Manages an analog output (AO).
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
//      ao           <Number>    Analog Output location (IoData AO's channel)
//      channel      <Number>    Device channel number
//      offset       <Number>    Offset value (default: 0.0)
//      gain         <Number>    Gain value   (default: 1.0)
//      table        <Table1>    Shaping function table (default: none)
//
//------------------------------------------------------------------------------
class AnalogOutput final: public AbstractAdapter
{
   DECLARE_SUBCLASS(AnalogOutput, AbstractAdapter)

public:
   AnalogOutput();


   int getLocation() const                             { return location;   }
   int getChannel() const                              { return channel;    }
   double getOffset() const                            { return offset;     }
   double getGain() const                              { return gain;       }
   const base::Table1* getTable() const                { return table;      }

   bool setLocation(const int x)                       { location = x;  return true; }
   bool setChannel(const int x)                        { channel = x;   return true; }
   bool setOffset(const double x)                      { offset = x;    return true; }
   bool setGain(const double x)                        { gain = x;      return true; }
   bool setTable(const base::Table1* const);

private:
   void processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData) final     {}
   void processOutputsImpl(const base::AbstractIoData* const outData, base::AbstractIoDevice* const device) final;

   int location {};               // AbstractIoData analog output channel number
   int channel {};                // Analog channel number
   double offset {};              // Offset:  value = gain * (vin - offset)
   double gain {1.0};             // Gain:    value = gain * (vin - offset)
   const base::Table1* table {};  // Shaping table

private:
   // slot table helper methods
   bool setSlotLocation(const base::Number* const);
   bool setSlotChannel(const base::Number* const);
   bool setSlotOffset(const base::Number* const);
   bool setSlotGain(const base::Number* const);
   bool setSlotTable(const base::Table1* const x)           {return setTable(x);}
};

}
}

#endif
