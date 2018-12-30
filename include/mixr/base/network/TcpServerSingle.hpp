
#ifndef __mixr_base_TcpServerSingle_HPP__
#define __mixr_base_TcpServerSingle_HPP__

#include "mixr/base/network/TcpHandler.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: TcpServerSingle
// Description: Server side of a single TCP/IP connection.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: TcpServerSingle
// Slots: none
//
// Example:
//
//        ( TcpServer
//            port: 2010    // Server port
//        )
//------------------------------------------------------------------------------
// Notes: initNetwork() will wait on a connection even if the 'noWaitFlag' is true.
//------------------------------------------------------------------------------
class TcpServerSingle final: public TcpHandler
{
   DECLARE_SUBCLASS(TcpServerSingle, TcpHandler)

public:
   TcpServerSingle();

   bool initNetwork(const bool noWaitFlag) final;

protected:
   bool bindSocket() final;

   virtual bool listenForConnections();
   virtual bool acceptConnection();
};

}
}

#endif

