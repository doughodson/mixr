
#ifndef __mixr_base_TcpServerMultiple_HPP__
#define __mixr_base_TcpServerMultiple_HPP__

#include "mixr/base/network/TcpHandler.hpp"

namespace mixr {
namespace base {
class Integer;

//------------------------------------------------------------------------------
// Class: TcpServerMultiple
// Description: Server side that can support multiple TCP/IP connections.
//              Our application must request each connection via acceptConnection(),
//              which will return a new instance of a TcpHandler when a connection
//              is made, or zero is return if no connection was made.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: TcpServerMultiple
// Slots:
//      backlog   <Integer>    ! Listen socket's max backlog (default: 1)
//
// Example:
//
//        ( TcpServerMultiple
//            port: 2010      // Server port
//            backlog: 10     // max client backlog
//        )
//------------------------------------------------------------------------------
// Note: The 'noWaitFlag' slot applies only to the new TcpHandler objects.
//       The functions initNetwork() and acceptConnection() are unblocked (or no
//       wait) I/O functions.
//------------------------------------------------------------------------------
class TcpServerMultiple final: public TcpHandler
{
   DECLARE_SUBCLASS(TcpServerMultiple, TcpHandler)

public:
   TcpServerMultiple();

   virtual TcpHandler* acceptConnection();

   unsigned int getBacklog() const        { return backlog; }

   bool initNetwork(const bool noWaitFlag) final;

protected:
   bool bindSocket() final;

   virtual bool setBacklog(const unsigned int);
   virtual bool listenForConnections();

private:
   int backlog{1};

private:
   // slot table helper methods
   bool setSlotBacklog(const Integer* const);
};

}
}

#endif

