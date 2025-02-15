
#ifndef __mixr_base_TcpClient_HPP__
#define __mixr_base_TcpClient_HPP__

#include "mixr/base/network/ITcpHandler.hpp"

#include <string>

namespace mixr {
namespace base {
class Identifier;
class String;

//------------------------------------------------------------------------------
// Class: TcpClient
// Description: Client side of a single TCP/IP connection.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: TcpClient
// Slots:
//      ipAddress  <String>     ! Destination IP address specified as a String (e.g., "111.122.133.144")
//                              ! (default: found via local host name)
//      ipAddress  <Identifier> ! Destination host name specified as an Identifier to be
//                              ! used to look up a particular address
//
// Example:
//
//        ( TcpClient
//             ipAddress: hostname    // Server host name
//             port: 2010             // Server port
//        )
//------------------------------------------------------------------------------
// Notes: initNetwork() will wait on a connection even if the 'noWaitFlag' is true.
//------------------------------------------------------------------------------
class TcpClient final: public ITcpHandler
{
    DECLARE_SUBCLASS(TcpClient, ITcpHandler)

public:
    TcpClient();

    bool initNetwork(const bool noWaitFlag) final;

protected:
    const char* getIpAddress()  { return ipAddr.c_str(); }
    bool connectToServer();      // Connect to the server

    bool init() final;
    bool bindSocket() final;

private:
    std::string ipAddr;    // host name or IP address

private:
    // slot table helper methods
    bool setSlotIpAddress(const String* const);
    bool setSlotIpAddress(const Identifier* const);
};

}
}

#endif

