
// M$ WinSock has slightly different return types, some different calling, and
// is missing some of the calls that are standard in Berkeley and POSIX socket
// implementation.  These slight differences will be handled in setting basic
// typedefs, defines, and constants that will make each convention match for
// use later in the code.  This will save a lot of pre-processor intervention
// and make the code that much more enjoyable to read!
#if defined(WIN32)
    #include <sys/types.h>
    #include <Winsock2.h>
    #define bzero(a,b)  ZeroMemory( a, b )
    typedef int socklen_t;
#else
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <sys/fcntl.h>
    #include <sys/ioctl.h>
    #ifdef sun
        #include <sys/filio.h> // -- added for Solaris 10
    #endif
//    static const int INVALID_SOCKET{-1}; // Always -1 and errno is set
//    static const int SOCKET_ERROR{-1};
#endif

#include "mixr/base/network/INetHandler.hpp"

#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(INetHandler, "INetHandler")
EMPTY_SLOTTABLE(INetHandler)
EMPTY_COPYDATA(INetHandler)
EMPTY_DELETEDATA(INetHandler)

// Byte order
bool INetHandler::netByteOrder {checkByteOrder()};

INetHandler::INetHandler()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// Initialize the network handler --
//------------------------------------------------------------------------------
bool INetHandler::initNetwork(const bool noWaitFlag)
{
    // initialize socket
    bool ok {init()};
    if (!ok) {
        std::cerr << "NetHandler::initNetwork(): init() FAILED" << std::endl;
    }

    return ok;
}

//------------------------------------------------------------------------------
// init() -- initialize the network
//------------------------------------------------------------------------------
bool INetHandler::init()
{
    bool ok {true};

#if defined(WIN32)
    // initialize Winsock2
    WSADATA wsaData;
    WORD wVersionRequested{MAKEWORD( 2, 2 )};
    // initiate the use of Winsock DLL
    int err{::WSAStartup( wVersionRequested, &wsaData )};
    if ((err != 0) && isMessageEnabled(MSG_ERROR)) {
        std::cerr << "NetHandler::init() -- WSAStartup() FAILED" << std::endl;
    }
    ok = (err == 0);
#endif

    return ok;
}

//------------------------------------------------------------------------------
// toNet() -- byte swaps a host to network buffer.  The buffer MUST consist
//            of 'nl' int (4 byte) words followed by 'ns' short (2 byte) words.
//            The parameters 'nl' and 'ns' can be zero.
//------------------------------------------------------------------------------
void INetHandler::toNet(const void* const hostData, void* const netData, const int nl, const int ns)
{
   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the source (this).
   auto psl = static_cast<const u_long*>(hostData);
   auto pss = reinterpret_cast<const u_short*>(psl + nl);

   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the destination (netData).
   u_long* pdl{static_cast<u_long*>(netData)};
   u_short* pds{(u_short*) (pdl + nl)};

   for (int i = 0; i < nl; i++) {
      const u_long kk{*psl++};
      u_long ll{htonl(kk)};
      *pdl++ = ll;
   }
   for (int i = 0; i < ns; i++) {
      //*pds++ = htons(*pss++);
      const u_short kk{*pss++};
      u_short ss{htons(kk)};
      *pds++ = ss;
   }
}

//------------------------------------------------------------------------------
// toHost() -- byte swaps a network to host buffer.  The buffer MUST consist
//            of 'nl' int (4 byte) words followed by 'ns' short (2 byte) words.
//            The parameters 'nl' and 'ns' can be zero.
//------------------------------------------------------------------------------
void INetHandler::toHost(const void* const netData, void* const hostData, const int nl, const int ns)
{
   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the source (this).
   auto psl = static_cast<const u_long*>(netData);
   auto pss = reinterpret_cast<const u_short*>(psl + nl);

   // Compute pointers to the int word (4 byte) and short
   // short word (2 byte) areas of the destination (hostData).
   auto pdl = static_cast<u_long*>(hostData);
   auto pds = reinterpret_cast<u_short*>(pdl + nl);

   for (int i = 0; i < nl; i++) {
      *pdl++ = ntohl(*psl++);
   }
   for (int i = 0; i < ns; i++) {
      *pds++ = ntohs(*pss++);
   }
}

//------------------------------------------------------------------------------
// checkByteOrder() -- Checks byte order and returns true if in network byte order
//------------------------------------------------------------------------------
bool INetHandler::checkByteOrder()
{
    unsigned short n1 {1};
    unsigned short n2 = htons(n1);
    return (n1 == n2);               // No difference? Then we already in network order!
}

}
}

