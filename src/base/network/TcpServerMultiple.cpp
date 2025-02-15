//------------------------------------------------------------------------------
// Class: TcpServerMultiple
//------------------------------------------------------------------------------
//
// M$ WinSock has slightly different return types, some different calling, and
// is missing some of the calls that are standard in Berkeley and POSIX socket
// implementation.  These slight differences will be handled in setting basic
// typedefs, defines, and constants that will make each convention match for
// use later in the code.  This will save a lot of pre-processor intervention
// and make the code that much more enjoyable to read!
//
#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #include <WS2tcpip.h>
    #define  bzero(a,b)   ZeroMemory(a,b)
    typedef int socklen_t;
#else
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
    static const int INVALID_SOCKET{-1}; // Always -1 and errno is set
    static const int SOCKET_ERROR{-1};
#endif

#include "mixr/base/network/ITcpHandler.hpp"
#include "mixr/base/network/TcpServerMultiple.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"

#include <cstdio>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(TcpServerMultiple, "TcpServerMultiple")

BEGIN_SLOTTABLE(TcpServerMultiple)
    "backlog",    // 1) Listen socket's max backlog (default: 1)
END_SLOTTABLE(TcpServerMultiple)

BEGIN_SLOT_MAP(TcpServerMultiple)
    ON_SLOT(1, setSlotBacklog, Integer)
END_SLOT_MAP()

EMPTY_DELETEDATA(TcpServerMultiple)

TcpServerMultiple::TcpServerMultiple()
{
   STANDARD_CONSTRUCTOR()
}

void TcpServerMultiple::copyData(const TcpServerMultiple& org, const bool)
{
    BaseClass::copyData(org);
    backlog = org.backlog;
}

//------------------------------------------------------------------------------
// Initialize this multicast handler --
//------------------------------------------------------------------------------
bool TcpServerMultiple::initNetwork(const bool noWaitFlag)
{
    noWait = noWaitFlag;
    bool ok{BaseClass::initNetwork(false)};
    if (ok) {
        ok = listenForConnections();
    }
    return ok;
}

// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers.
// -------------------------------------------------------------
bool TcpServerMultiple::bindSocket()
{
   // ---
   // Our base class will bind the socket
   // ---
   bool ok{BaseClass::bindSocket()};

   if (ok) {
      struct sockaddr_in addr;        // Working address structure
      bzero(&addr, sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = INADDR_ANY;
      if (getLocalAddr() != 0) addr.sin_addr.s_addr = getLocalAddr ();
      if (getLocalPort() != 0) addr.sin_port = htons (getLocalPort());
      else addr.sin_port = htons(getPort());

      // Only in server do we bind
      if (::bind(socketNum, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
         std::perror("TcpHandler::bindSocket(): bind error");
         return false;
      }

      if (!setSendBuffSize()) return false;

      if (!setRecvBuffSize()) return false;
   }

   return ok;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
bool TcpServerMultiple::listenForConnections()
{
    if (socketNum == INVALID_SOCKET) return false;
    if (::listen(socketNum, getBacklog()) == SOCKET_ERROR) {
        std::perror("TcpHandler::listenForConnections(): error! \n");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
// listenForConnections() -- puts the socket into listen mode
//------------------------------------------------------------------------------
ITcpHandler* TcpServerMultiple::acceptConnection()
{
   struct sockaddr_in clientAddr;

   socklen_t cAddrSize{sizeof(clientAddr)};
   LcSocket newSocket{INVALID_SOCKET};

   if (isMessageEnabled(MSG_INFO)) {
       std::cout << "Waiting to accept connection on " << getPort() << " ... " << std::endl;
   }
   newSocket = ::accept(socketNum, reinterpret_cast<struct sockaddr*>(&clientAddr), &cAddrSize);
   // Since INVALID_SOCKET is defined as -1 for POSIX, ::accept will return
   // INVALID_SOCKET as the error condition (see MSDN help and POSIX man pages
   // for more information).
   ITcpHandler* newHandler{};
   if (newSocket != INVALID_SOCKET) {
      newHandler = new ITcpHandler(newSocket);

      // Set blocked or no-wait
      if (noWait) newHandler->setNoWait();
      else newHandler->setBlocked();
   }

   return newHandler;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool TcpServerMultiple::setBacklog(const unsigned int value)
{
   backlog = value;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool TcpServerMultiple::setSlotBacklog(const Integer* const x)
{
   return setBacklog(x->asInt());
}

}
}
