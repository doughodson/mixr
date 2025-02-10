
#ifndef __mixr_linkage_DiscreteInput_HPP__
#define __mixr_linkage_DiscreteInput_HPP__

#include "mixr/linkage/adapters/IAdapter.hpp"

namespace mixr {
namespace base { class IIoData; class IIoDevice; class Boolean; class Integer; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: DiscreteInput
//
// Description: Manage discrete inputs (DI).
//
// Slots:
//      di           <Integer>   ! Discrete Input location (IoData's DI channel)
//      channel      <Integer>   ! Device channel (bit) number on the port
//      port         <Integer>   ! Device port number (default: 0)
//      inverted     <Boolean>   ! Inverted bit flag (default: false)
//      value        <Boolean>   ! Initial value (default: false)
//      count        <Integer>   ! Number of DIs to manage starting at 'di' and
//                               !   'channel'; port is unchanged (default: 1)
//
// Note: If 'count' is less than zero then the DIs are stored in reverse
//       order (i.e., 'di' location is decremented)
//------------------------------------------------------------------------------
class DiscreteInput final: public IAdapter
{
   DECLARE_SUBCLASS(DiscreteInput, IAdapter)

public:
   DiscreteInput();

   int getLocation() const                         { return location;  }
   int getPort() const                             { return port;      }
   int getChannel() const                          { return channel;   }
   bool getInvert() const                          { return invert;    }
   bool getValue() const                           { return value;     }
   int getCount() const                            { return count;     }

   bool setLocation(const int x)                   { location = x;     return true; }
   bool setPort(const int x)                       { port = x;     devEnb = true;    return true; }
   bool setChannel(const int x)                    { channel = x;  devEnb = true;    return true; }
   bool setInvert(const bool x)                    { invert = x;       return true; }
   bool setValue(const bool x)                     { value = x;        return true; }
   bool setCount(const int x)                      { count = x;        return true; }

private:
   void processInputsImpl(const base::IIoDevice* const device, base::IIoData* const inData) final;
   void processOutputsImpl(const base::IIoData* const outData, base::IIoDevice* const device) final   {}

   int location {};      // AbstractIoData input bit location
   int port {};          // Port number
   int channel {};       // Port's channel (bit) number
   bool devEnb {};       // Device enabled
   bool invert {};       // Inverted bit flag
   bool value {};        // Initial value
   int count {1};        // Number of DIs (neg for reverse order)

private:
   // slot table helper methods
   bool setSlotLocation(const base::Integer* const);
   bool setSlotPort(const base::Integer* const);
   bool setSlotChannel(const base::Integer* const);
   bool setSlotInverted(const base::Boolean* const);
   bool setSlotValue(const base::Boolean* const);
   bool setSlotCount(const base::Integer* const);
};

}
}

#endif
