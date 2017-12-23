
#ifndef __mixr_base_UdpBroadcastHandler_H__
#define __mixr_base_UdpBroadcastHandler_H__

#include "mixr/base/network/PosixHandler.hpp"

namespace mixr {
namespace base {

class String;
class Number;

//------------------------------------------------------------------------------
// Class: UdpBroadcastHandler
//
// Description: UDP broadcast network handler
//
// Factory name: UdpBroadcastHandler
//
// Slots:
//       networkMask  <String>    ! Host Net Mask   "255.255.255.255"
//
// Input File Example:
//
//        ( UdpBroadcastHandler
//           localIpAddress: "224.0.0.251"  // Local host IP address
//           networkMask: "255.255.255.0"   // Local host Network Mask
//           port: 2010                     // Destination port
//           localPort: 2011                // Port to send from
//           shared: 1                      // Shared port
//        )
//
//------------------------------------------------------------------------------
class UdpBroadcastHandler : public PosixHandler
{
    DECLARE_SUBCLASS(UdpBroadcastHandler, PosixHandler)

public:
    UdpBroadcastHandler();

protected:
    bool init() override;
    bool bindSocket() override;

private:
    char* networkMask {};
    
private:
    // slot table helper methods
    bool setSlotNetworkMask(const String* const);
};

}
}

#endif

