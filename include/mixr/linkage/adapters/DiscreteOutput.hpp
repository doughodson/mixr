
#ifndef __mixr_linkage_DiscreteOutput_H__
#define __mixr_linkage_DiscreteOutput_H__

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

   bool setLocation(const int x)                  { location = x;      return true; }
   bool setPort(const int x)                      { port = x;          return true; }
   bool setChannel(const int x)                   { channel = x;       return true; }
   bool setInvertFlag(const bool x)               { invert = x;        return true; }

private:
   void processInputsImpl(const base::AbstractIoDevice* const device, base::AbstractIoData* const inData) final     {}
   void processOutputsImpl(const base::AbstractIoData* const outData, base::AbstractIoDevice* const device) final;

   int location{};     // AbstractIoData output bit location
   int port{};         // Port number
   int channel{};      // Port's channel (bit) number
   bool invert{};      // Inverted bit flag

private:
   // slot table helper methods
   bool setSlotLocation(const base::Integer* const);
   bool setSlotPort(const base::Integer* const);
   bool setSlotChannel(const base::Integer* const);
   bool setSlotInverted(const base::Boolean* const);
};

}
}

#endif
