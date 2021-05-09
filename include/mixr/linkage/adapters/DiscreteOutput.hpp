
#ifndef __mixr_linkage_DiscreteOutput_HPP__
#define __mixr_linkage_DiscreteOutput_HPP__

#include "mixr/linkage/adapters/AbstractAdapter.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; class Boolean; class Integer; class NetHandler; }
namespace linkage {

//------------------------------------------------------------------------------
// Class:  DiscreteOutput
//
// Description:  Manage discrete outputs (DO).
//
// Slots:
//      do           <Integer>   ! Discrete Output location (IoData's DO channel)
//      port         <Integer>   ! Device port number (default: 0)
//      channel      <Integer>   ! Device channel (bit) number on the port
//      inverted     <Boolean>   ! Inverted bit flag (default: false)
//      value        <Boolean>   ! Initial value (default: false)
//      count        <Integer>   ! Number of DOs to manage starting at 'do' and
//                               !   'channel'; port is unchanged (default: 1)
//
// Note: If 'count' is less than zero then the DOs are stored in reverse
//       order (i.e., 'do' location is decremented)
//------------------------------------------------------------------------------
class DiscreteOutput final: public AbstractAdapter
{
   DECLARE_SUBCLASS(DiscreteOutput, AbstractAdapter)

public:
   DiscreteOutput();

   int getLocation() const                        { return location;   }
   int getPort() const                            { return port;       }
   int getChannel() const                         { return channel;    }
   bool getInvertFlag() const                     { return invert;     }
   bool getValue() const                          { return value;      }
   int getCount() const                           { return count;      }

   bool setLocation(const int x)                  { location = x;      return true; }
   bool setPort(const int x)                      { port = x;     devEnb = true;     return true; }
   bool setChannel(const int x)                   { channel = x;  devEnb = true;     return true; }
   bool setInvertFlag(const bool x)               { invert = x;        return true; }
   bool setValue(const bool x)                    { value = x;         return true; }
   bool setCount(const int x)                     { count = x;         return true; }

private:
   void processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData) final     {}
   void processOutputsImpl(const base::AbstractIoData* const outData, base::AbstractIoDevice* const device) final;

   int location{};     // AbstractIoData output bit location
   int port{};         // Port number
   int channel{};      // Port's channel (bit) number
   bool devEnb{};      // Device enabled
   bool invert{};      // Inverted bit flag
   bool value{};       // Initial value
   int count{1};       // Number of DOs (neg for reverse order)

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
