
#ifndef __mixr_base_UdpBroadcastHandler_HPP__
#define __mixr_base_UdpBroadcastHandler_HPP__

#include "mixr/base/network/IPosixHandler.hpp"

#include <string>

namespace mixr {
namespace base {
class String;
class Number;

//------------------------------------------------------------------------------
// Class: UdpBroadcastHandler
// Description: UDP broadcast network handler
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: UdpBroadcastHandler
// Slots:
//       networkMask  <String>    ! host network mask  (e.g., "255.255.255.255")
//
// Example:
//
//        ( UdpBroadcastHandler
//           localIpAddress: "224.0.0.251"  // Local host IP address
//           networkMask: "255.255.255.0"   // Local host Network Mask
//           port: 2010                     // Destination port
//           localPort: 2011                // Port to send from
//           shared: true                   // Shared port
//        )
//------------------------------------------------------------------------------
class UdpBroadcastHandler final: public IPosixHandler
{
    DECLARE_SUBCLASS(UdpBroadcastHandler, IPosixHandler)

public:
    UdpBroadcastHandler();

protected:
    bool init() final;
    bool bindSocket() final;

private:
    std::string networkMask;

private:
    // slot table helper methods
    bool setSlotNetworkMask(const String* const);
};

}
}

#endif

