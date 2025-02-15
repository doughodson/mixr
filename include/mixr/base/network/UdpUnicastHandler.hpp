
#ifndef __mixr_base_UdpUnicastHandler_HPP__
#define __mixr_base_UdpUnicastHandler_HPP__

#include "mixr/base/network/IPosixHandler.hpp"

#include <string>

namespace mixr {
namespace base {
class Identifier;
class String;

//------------------------------------------------------------------------------
// Class: UdpUnicastHandler
// Description: UDP/IP unicast network handler.  Sends unicast packets to the
//              destination IP and port.
//
//              The destination port can be set by the input file, or by using
//              the setNetAddr() and setPort() functions prior to calling the
//              sendData() function, or by the using sendDataTo() function,
//              which changes to destination IP and port only for the one packet.
//
//              The local port must be defined if you're going to be changing
//              the destination port.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: UdpUnicastHandler
// Slots:
//      ipAddress  <String>     ! Destination host name or IP address "111.122.133.144" string
//                              ! (default: found via local host name)
//      ipAddress  <Identifier> ! Destination host name identifier (e.g., 'localhost') to be
//                              ! used to look up a particular address
//
// Example:
//
//        ( UdpUnicastHandler
//           ipAddress: hostname    // Destination host name
//           port: 2010             // Destination port
//           localPort: 2011        // Local port to send from
//        )
//------------------------------------------------------------------------------
class UdpUnicastHandler final: public IPosixHandler
{
   DECLARE_SUBCLASS(UdpUnicastHandler, IPosixHandler)

public:
   UdpUnicastHandler();

   // Sets the destination IP address (all future packets)
   bool setNetAddr(const uint32_t netAddr)      { return BaseClass::setNetAddr(netAddr); }

   // Sets the destination IP address by host name (all future packets)
   bool setNetAddr(const char* const hostname)  { return BaseClass::setNetAddr(hostname); }

   // Sets the destination port number (all future packets)
   bool setPort(const uint16_t n)               { return BaseClass::setPort(n); }

   // Send data to a specific IP/Port
   virtual bool sendDataTo(
         const char* const packet,  // Data packet
         const int size,            // Size of the data packet
         const uint32_t ip,         // Destination IP address (this packet only)
         const uint16_t port        // Destination port (this packet only)
      );

protected:
   bool init() final;
   bool bindSocket() final;

private:
   std::string ipAddr;   // host name or IP address

private:
   // slot table helper methods
   bool setSlotIpAddress(const String* const);
   bool setSlotIpAddress(const Identifier* const);
};

}
}

#endif

