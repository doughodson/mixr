
#ifndef __mixr_base_TcpHandler_HPP__
#define __mixr_base_TcpHandler_HPP__

#include "mixr/base/network/IPosixHandler.hpp"

namespace mixr {
namespace base {

class String;

//------------------------------------------------------------------------------
// Class: TcpHandler
// Description: Abstract class to support TCP/IP connections
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: TcpHandler
// Slots: none
//------------------------------------------------------------------------------
class TcpHandler : public IPosixHandler
{
   DECLARE_SUBCLASS(TcpHandler, IPosixHandler)

public:
   TcpHandler();

   bool hasBeenTerminated()          { return connectionTerminated; }

   bool sendData(const char* const packet, const int size) final;
   unsigned int recvData(char* const packet, const int maxSize) final;
   bool isConnected() const final;
   bool closeConnection() final;

   // "Socket Only" constructor that will be used by one of our derived classes;
   // The socket should be an AF_INET, SOCK_STREAM type socket that has been
   // initialized, bound, connected, and is ready for use.
   TcpHandler(const LcSocket socketNum);

protected:
   bool init() override;

   bool connected{};              // true if we are connected
   bool connectionTerminated{};   // true if we had a connection, then lost it (or terminated it ourselves)

   bool  noWait{};                // TCP's no wait I/O flag
};

}
}

#endif

