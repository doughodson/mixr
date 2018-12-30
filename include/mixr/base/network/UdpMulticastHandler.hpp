
#ifndef __mixr_base_UdpMulticastHandler_HPP__
#define __mixr_base_UdpMulticastHandler_HPP__

#include "mixr/base/network/PosixHandler.hpp"

#include <string>

namespace mixr {
namespace base {
class Boolean;
class Integer;
class String;

//------------------------------------------------------------------------------
// Class: UdpMulticastHandler
// Description: UDP multicast network handler.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: UdpMulticastHandler
// Slots:
//      multicastGroup  <String>    ! String containing the multicast IP address in
//                                  ! the Internet standard "." (dotted) notation.
//                                  ! IP multicast addresses range from 224.0.0.0
//                                  ! through 239.255.255.255 (e.g., "225.0.0.251")
//
//      ttl             <Integer>   ! Multicast Time-To-Live (TTL) value (default: 1)
//
//      loopback        <Boolean>   ! Multicast Loopback flag (default: true)
//
// Example:
//
//        ( UdpMulticastHandler
//           multicastGroup: "224.0.0.251"      // Multicast group
//           port: 2010                         // Multicast port
//           localPort: 2011                    // Port to send from
//           shared: true                       // Shared socket
//           ttl:    4                          // Time-to-live
//           loopback: true                     // Loop back
//        )
//------------------------------------------------------------------------------
// Notes:
//
// Multicast:
// "A point-to-many networking model in which a packet is sent to a specific address,
//  and only those computers that are set to receive information from this address
//  receive the packet. On the Internet, the possible IP multicast addresses range
//  from 224.0.0.0 through 239.255.255.255. Computer networks typically use a unicast
//  model, in which a different version of the same packet is sent to each address
//  that must receive it. The multicast model greatly reduces traffic and increases
//  efficiency on such networks."
//
// From Windows documentation:
//    "Note  The Winsock version of the IP_MULTICAST_LOOP option is semantically
//    different than the UNIX version of the IP_MULTICAST_LOOP option:
//    -> In Winsock, the IP_MULTICAST_LOOP option applies only to the receive path.
//    -> In the UNIX version, the IP_MULTICAST_LOOP option applies to the send path.
//    -> For example, applications ON and OFF (which are easier to track than X and Y)
//       join the same group on the same interface; application ON sets the IP_MULTICAST_LOOP
//       option on, application OFF sets the IP_MULTICAST_LOOP option off. If ON and OFF are
//       Winsock applications, OFF can send to ON, but ON cannot sent to OFF. In contrast,
//       if ON and OFF are UNIX applications, ON can send to OFF, but OFF cannot send to ON."
//------------------------------------------------------------------------------
class UdpMulticastHandler final: public PosixHandler
{
    DECLARE_SUBCLASS(UdpMulticastHandler, PosixHandler)

public:
    UdpMulticastHandler();

    int getTTL() const                      { return ttl; }
    void setTTL(const int v)                { ttl = v; }

    bool getLoopback() const                { return loopback; }
    void setLoopback(const bool b)          { loopback = b; }

    bool initNetwork(const bool noWaitFlag) final;
    bool isConnected() const final;
    bool closeConnection() final;

protected:
    virtual bool joinTheGroup();

    bool init() final;
    bool bindSocket() final;

private:
    std::string multicastGroup;      // Multicast Group Name
    int  ttl{1};                     // Time-to-live value
    bool loopback{true};             // Loop back flag
    bool initialized{};              // handler has been initialized

private:
    // slot table helper methods
    bool setSlotMulticastGroup(const String* const);
    bool setSlotTTL(const Integer* const);
    bool setSlotLoopback(const Boolean* const);
};

}
}

#endif
