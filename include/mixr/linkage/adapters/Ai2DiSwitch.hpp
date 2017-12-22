
#ifndef __mixr_linkage_Ai2DiSwitch_H__
#define __mixr_linkage_Ai2DiSwitch_H__

#include "mixr/linkage/adapters/AbstractAdapter.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; class Number; }
namespace linkage {

//------------------------------------------------------------------------------
// Class:  Ai2DiSwitch
//
// Description: Signal converter: switches a DI based on an analog input.
//              The DI will become true if the analog input becomes greater
//              than or equal to the 'level' value, otherwise the DI is set to
//              false.  If the 'inverted' flag is true then the DI's value is
//              inverted before being set.
//
//    Analog      Invert      DI
//    input       flag        value
//    ----------  ----------  -----------
//      < level      false      false
//     >= level      false      true
//      < level      true       true
//     >= level      true       false
//
//
// Factory name: Ai2DiSwitch
// Slots:
//      di           <Number>    Discrete Input location (IoData's DI channel)
//      channel      <Number>    Device's AI channel number
//      level        <Number>    Level to switch DI (default: 0)
//      inverted     <Boolean>   Inverted bit flag (default: false)
//
//------------------------------------------------------------------------------
class Ai2DiSwitch final: public AbstractAdapter
{
   DECLARE_SUBCLASS(Ai2DiSwitch, AbstractAdapter)

public:
   Ai2DiSwitch();

   int getLocation() const                        { return location;  }
   int getChannel() const                         { return channel;   }
   double getLevel() const                        { return level;     }
   bool getInvertFlag() const                     { return invert;    }

   bool setLocation(const int x)                  { location = x;    return true; }
   bool setChannel(const int x)                   { channel = x;     return true; }
   bool setLevel(const double x)                  { level = x;       return true; }
   bool setInvertFlag(const bool x)               { invert = x;      return true; }

private:
   void processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData) final;
   void processOutputsImpl(const base::AbstractIoData* const outData, base::AbstractIoDevice* const device) final   {}

   int location {};     // AbstractIoData input bit location
   int channel {};      // Port's channel (bit) number
   double level {};     // Switching level
   bool invert {};      // Inverted bit flag

private:
   // slot table helper methods
   bool setSlotLocation(const base::Number* const);
   bool setSlotChannel(const base::Number* const);
   bool setSlotLevel(const base::Number* const);
   bool setSlotInverted(const base::Number* const);
};

}
}

#endif
