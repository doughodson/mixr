
#ifndef __mixr_linkage_DiscreteInput_H__
#define __mixr_linkage_DiscreteInput_H__

#include "mixr/linkage/adapters/AbstractAdapter.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; class Number; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: DiscreteInput
//
// Description:  Manage discrete inputs (DI).
//
// Slots:
//      di           <Number>    Discrete Input location (IoData's DI channel)
//      channel      <Number>    Device channel (bit) number on the port
//      port         <Number>    Device port number (default: 0)
//      inverted     <Boolean>   Inverted bit flag (default: false)
//------------------------------------------------------------------------------
class DiscreteInput final: public AbstractAdapter
{
   DECLARE_SUBCLASS(DiscreteInput, AbstractAdapter)

public:
   DiscreteInput();

   int getLocation() const                         { return location;  }
   int getPort() const                             { return port;      }
   int getChannel() const                          { return channel;   }
   bool getInvert() const                          { return invert;    }

   bool setLocation(const int x)                   { location = x;     return true; }
   bool setPort(const int x)                       { port = x;         return true; }
   bool setChannel(const int x)                    { channel = x;      return true; }
   bool setInvert(const bool x)                    { invert = x;       return true; }

private:
   void processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData) final;
   void processOutputsImpl(const base::AbstractIoData* const outData, base::AbstractIoDevice* const device) final   {}

   int location {};      // AbstractIoData input bit location
   int port {};          // Port number
   int channel {};       // Port's channel (bit) number
   bool invert {};       // Inverted bit flag

private:
   // slot table helper methods
   bool setSlotLocation(const base::Number* const);
   bool setSlotPort(const base::Number* const);
   bool setSlotChannel(const base::Number* const);
   bool setSlotInverted(const base::Number* const);
};

}
}

#endif
