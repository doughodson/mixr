
#ifndef __mixr_base_TcpClient_H__
#define __mixr_base_TcpClient_H__

#include "mixr/base/network/TcpHandler.hpp"

#include <string>

namespace mixr {
namespace base {
class Identifier;
class String;

//------------------------------------------------------------------------------
// Class: TcpClient
// Description: Client side of a single TCP/IP connection.
//------------------------------------------------------------------------------
// Factory name: TcpClient
//------------------------------------------------------------------------------
// Slots:
//      ipAddress  <String>     ! Destination IP address specified as a String (e.g., "111.122.133.144")
//                              ! (default: found via local host name)
//      ipAddress  <Identifier> ! Destination host name specified as an Identifier to be
//                              ! used to look up a particular address
//------------------------------------------------------------------------------
// Notes:
//
// Input File Example:
//
//        ( TcpClient
//             ipAddress: hostname    // Server host name
//             port: 2010             // Server port
//        )
//
// Note: initNetwork() will wait on a connection even if the 'noWaitFlag' is true.
//------------------------------------------------------------------------------
class TcpClient : public TcpHandler
{
    DECLARE_SUBCLASS(TcpClient, TcpHandler)

public:
    TcpClient();

    bool initNetwork(const bool noWaitFlag) override;

protected:
    const char* getIpAddress()  { return ipAddr.c_str(); }
    bool connectToServer();      // Connect to the server

    bool init() override;

    bool bindSocket() override;

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

