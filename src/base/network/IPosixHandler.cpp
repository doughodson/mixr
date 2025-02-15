
#if defined(WIN32)
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
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
    static const int INVALID_SOCKET{-1}; // Always -1 and errno is set
    static const int SOCKET_ERROR{-1};
#endif

#include "mixr/base/network/IPosixHandler.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(IPosixHandler, "IPosixHandler")
EMPTY_DELETEDATA(IPosixHandler)

BEGIN_SLOTTABLE(IPosixHandler)
    "localIpAddress",       // 1) String containing the local host's name or its IP
                            //    address in the Internet standard "." (dotted) notation.
                            //    (default: found via local host name)
    "localPort",            // 2) (optional) Local port number
    "port",                 // 3) Number of the port we're connecting to (required)
    "shared",               // 4) Shared (reuse) flag [default: false (not shared)]
    "sendBuffSizeKb",       // 5) Send buffer size in KB's    (default:  32 Kb; max 1024)
    "recvBuffSizeKb",       // 6) Receive buffer size in KB's (default: 128 Kb; max 1024)
    "ignoreSourcePort",     // 7) Ignore message from this source port
END_SLOTTABLE(IPosixHandler)

BEGIN_SLOT_MAP(IPosixHandler)
    ON_SLOT(1, setSlotLocalIpAddress,   Identifier)
    ON_SLOT(1, setSlotLocalIpAddress,   String)
    ON_SLOT(2, setSlotLocalPort,        Integer)
    ON_SLOT(3, setSlotPort,             Integer)
    ON_SLOT(4, setSlotShared,           Boolean)
    ON_SLOT(5, setSlotSendBuffSize,     Integer)
    ON_SLOT(6, setSlotRecvBuffSize,     Integer)
    ON_SLOT(7, setSlotIgnoreSourcePort, Integer)
END_SLOT_MAP()

IPosixHandler::IPosixHandler():localAddr(INADDR_ANY), netAddr(INADDR_ANY), fromAddr1(INADDR_NONE)
{
   STANDARD_CONSTRUCTOR()

   // Since INVALID_SOCKET is set to the invalid socket state above for both
   // WIN32 and POSIX, there is no need to differentiate between them at this
   // level or further in the source.
   socketNum = INVALID_SOCKET;
}

void IPosixHandler::copyData(const IPosixHandler& org, const bool)
{
    BaseClass::copyData(org);

    port = org.port;
    localPort = org.localPort;
    ignoreSourcePort = org.ignoreSourcePort;
    sendBuffSizeKb = org.sendBuffSizeKb;
    recvBuffSizeKb = org.recvBuffSizeKb;
    setSharedFlag(org.getSharedFlag());
    netAddr = org.netAddr;
    localAddr = org.localAddr;
    initialized = org.initialized;

    localIpAddr = org.localIpAddr;
}

//------------------------------------------------------------------------------
// Initialize the network handler --
//------------------------------------------------------------------------------
bool IPosixHandler::initNetwork(const bool noWaitFlag)
{
    // Initialize socket
    bool ok{init()};
    if (ok) {
        // Bind and configure socket
        ok = bindSocket();
        if (ok) {
            if (noWaitFlag) {
                ok = setNoWait();
                if (!ok) std::cerr << "PosixHandler::initNetwork(): setNoWait() FAILED" << std::endl;
            } else {
                ok = setBlocked();
                if (!ok) std::cerr << "PosixHandler::initNetwork(): setBlocked() FAILED" << std::endl;
            }
        } else {
            std::cerr << "PosixHandler::initNetwork(): bindSocket() FAILED" << std::endl;
        }
    } else {
        std::cerr << "PosixHandler::initNetwork(): init() FAILED" << std::endl;
    }

    initialized = ok;
    if (initialized && isMessageEnabled(MSG_DEBUG)) {
        std::cout << "PosixHandler::initNetwork() -- network initialized successfully" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// init() -- initialize the network
//------------------------------------------------------------------------------
bool IPosixHandler::init()
{
    bool ok{BaseClass::init()};

    // ---
    // Set the local IP address
    // ---
    if (!localIpAddr.empty()) {
        setLocalAddr(localIpAddr.c_str());
    }

    return ok;
}

// -------------------------------------------------------------
// bindSocket() -- bind the socket to an address, and configure
// the send and receive buffers.
// -------------------------------------------------------------
bool IPosixHandler::bindSocket()
{
    if (socketNum == INVALID_SOCKET) return false;

    // ---
    // Set the reuse socket attribute
    // ---
    {
#if defined(WIN32)
        BOOL optval{getSharedFlag()};
        socklen_t optlen{sizeof(optval)};
        if (::setsockopt(socketNum, SOL_SOCKET, SO_REUSEADDR, (const char*) &optval, optlen) == SOCKET_ERROR) {
#else
        int optval{getSharedFlag()};
        socklen_t optlen{sizeof(optval)};
        if (::setsockopt(socketNum, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) == SOCKET_ERROR) {
#endif
            std::perror("PosixHandler::bindSocket(): error setsockopt(SO_REUSEADDR)\n");
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
// Set the output buffer size
//------------------------------------------------------------------------------
bool IPosixHandler::setSendBuffSize()
{
   if (socketNum == INVALID_SOCKET) return false;

   const unsigned int optval{sendBuffSizeKb * 1024};
   socklen_t optlen{sizeof(optval)};
#if defined(WIN32)
   if (::setsockopt(socketNum, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&optval), optlen) == SOCKET_ERROR) {
#else
   if (::setsockopt(socketNum, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const void*>(&optval), optlen) == SOCKET_ERROR) {
#endif
      std::perror("PosixHandler::setSendBuffSize(): error setting the send buffer size\n");
      return false;
   }
   return true;
}

//------------------------------------------------------------------------------
// Sets the input buffer size
//------------------------------------------------------------------------------
bool IPosixHandler::setRecvBuffSize()
{
   if (socketNum == INVALID_SOCKET) return false;

   const unsigned int optval{recvBuffSizeKb * 1024};
   socklen_t optlen{sizeof(optval)};
#if defined(WIN32)
   if (::setsockopt(socketNum, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&optval), optlen) == SOCKET_ERROR) {
#else
   if (::setsockopt(socketNum, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const void*>(&optval), optlen) == SOCKET_ERROR) {
#endif
      std::perror("PosixHandler::setRecvBuffSize(): error setting the receive buffer size\n");
      return false;
   }
   return true;
}

// -------------------------------------------------------------
// setBlocked() -- Sets blocked I/O mode
// -------------------------------------------------------------
bool IPosixHandler::setBlocked()
{
    if (socketNum == NET_INVALID_SOCKET) return false;

// Set the socket 'sock' to Blocking. Wait I/O.
#if defined(WIN32)
    unsigned long zz{};
    if (::ioctlsocket(socketNum, FIONBIO, &zz) == SOCKET_ERROR) {
        std::perror("PosixHandler::setBlocked()");
        return false;
    }
#else
    const int zz{};
    if (::ioctl(socketNum, FIONBIO, &zz) == SOCKET_ERROR) {
        std::perror("PosixHandler::setBlocked()");
        return false;
    }
#endif

   return true;
}

// -------------------------------------------------------------
// setNoWait() -- Sets no wait (non-blocking) I/O mode
// -------------------------------------------------------------
bool IPosixHandler::setNoWait()
{
    if (socketNum == NET_INVALID_SOCKET) return false;

// Set the socket 'sock' to Non-Blocking. Nowait I/O.
#if defined(WIN32)
    unsigned long zz{true};
    if (::ioctlsocket(socketNum, FIONBIO, &zz ) == SOCKET_ERROR) {
        std::perror("PosixHandler::setNoWait()");
        return false;
    }
#else
    const int zz{1};
    if (::ioctl(socketNum, FIONBIO, &zz ) == SOCKET_ERROR) {
        std::perror("PosixHandler::setNoWait()");
        return false;
    }
#endif

   return true;
}

// -------------------------------------------------------------
// Returns true if the network handler has been initialized
// -------------------------------------------------------------
bool IPosixHandler::isConnected() const
{
    return initialized;
}

// -------------------------------------------------------------
// Close (un-initialize) this network
// -------------------------------------------------------------
bool IPosixHandler::closeConnection()
{
    initialized = false;
    return true;
}

// -------------------------------------------------------------
// sendData() -- Send data
// -------------------------------------------------------------
bool IPosixHandler::sendData(const char* const packet, const int size)
{
    if (socketNum == INVALID_SOCKET) return false;

    // Send the data
    struct sockaddr_in addr;        // Working address structure
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = netAddr;
    addr.sin_port = htons(port);
    socklen_t addrlen{sizeof(addr)};
    long result{::sendto(socketNum, packet, size, 0, reinterpret_cast<const struct sockaddr*>(&addr), addrlen)};
    if (result == SOCKET_ERROR) {
#if defined(WIN32)
        int err{::WSAGetLastError()};
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "PosixHandler::sendData(): sendto error: " << err << " hex=0x" << std::hex << err << std::dec << std::endl;
        }
#else
        std::perror("PosixHandler::sendData(): sendto error msg");
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "PosixHandler::sendData(): sendto error result: " << result << std::endl;
        }
#endif
        return false;
    }
    return true;
}

// -------------------------------------------------------------
// recvData() -- Receive data and possible ignore our own
//               local port messages.
// -------------------------------------------------------------
unsigned int IPosixHandler::recvData(char* const packet, const int maxSize)
{
   unsigned int n{};

   if (socketNum == INVALID_SOCKET) return 0;

   fromAddr1 = INADDR_NONE;
   fromPort1 = 0;

   bool tryAgain{true};
   while (tryAgain) {
      tryAgain = false;

      // Try to receive the data
      struct sockaddr_in raddr;       // IP address
      socklen_t addrlen{sizeof(raddr)};
      long result{::recvfrom(socketNum, packet, maxSize, 0, reinterpret_cast<struct sockaddr*>(&raddr), &addrlen)};

      if (result > 0 && ignoreSourcePort != 0) {
         // Ok we have one; make sure it's not one we should ignore
         std::uint16_t rport{ntohs(raddr.sin_port)};
         if (rport == ignoreSourcePort) {
            tryAgain = true;
         }
      }

      // set number of bytes received
      if (result > 0 && !tryAgain) {
         n = result;
         fromAddr1 = raddr.sin_addr.s_addr;
         fromPort1 = ntohs(raddr.sin_port);
      }
   }
   return n;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Set the shared flag
void IPosixHandler::setSharedFlag(const bool b)
{
   sharedFlg = b;
}

// Set port number
bool IPosixHandler::setPort(const uint16_t n1)
{
   port = n1;
   return true;
}

// Set local (source) port number
bool IPosixHandler::setLocalPort(const uint16_t n1)
{
   localPort = n1;
   return true;
}

// Sets the network IP address
bool IPosixHandler::setNetAddr(const uint32_t addr0)
{
    bool ok{};
    if (addr0 != INADDR_NONE) {
        netAddr = addr0;
        ok = true;
    }
    return ok;
}

// Sets the network IP address using hostname or the Internet standard "." (dotted) notation
bool IPosixHandler::setNetAddr(const char* const hostname)
{
    bool ok{};
    if (hostname != nullptr) {
        uint32_t addr0{INADDR_NONE};
        if (std::isdigit(hostname[0])) {
            // If 'hostname' starts with a number then first try to use it as an IP address
            addr0 = ::inet_addr(hostname);
            ok = (addr0 != INADDR_NONE);
        }
        if (addr0 == INADDR_NONE) {
            // Didn't work, try to find the host IP address by name
            if (isMessageEnabled(MSG_DEBUG)) {
               std::cout << "PosixHandler::setNetAddr(): Looking up host name: " << hostname;
            }
            const hostent* const p{gethostbyname(hostname)};
            if (p != nullptr && p->h_length > 0) {

                // 'q' points to the four byte address (in network order) as a single unsigned integer
                const unsigned int* const q{reinterpret_cast<const unsigned int*>(p->h_addr_list[0])};
                if (q != nullptr) {
                    struct in_addr in;
                    in.s_addr = *q;
                    addr0 = in.s_addr;
                    const char* const ipAddr{::inet_ntoa(in)};
                    if (ipAddr != nullptr) {
                        if (isMessageEnabled(MSG_DEBUG)) {
                           std::cout << " -- IP Address: " << ipAddr << std::endl;
                        }
                        ok = true;
                    }
                }
            }
            if (!ok && isMessageEnabled(MSG_DEBUG)) {
                std::cout << " -- HOST NOT FOUND!" << std::endl;
            }
        }
        if (addr0 != INADDR_NONE) ok = setNetAddr(addr0);
    }
    return ok;
}

// Sets the local IP address
bool IPosixHandler::setLocalAddr(const uint32_t addr0)
{
    bool ok{};
    if (addr0 != INADDR_NONE) {
        localAddr = addr0;
        ok = true;
    }
    return ok;
}

// Sets the local IP address using hostname or the Internet standard "." (dotted) notation
bool IPosixHandler::setLocalAddr(const char* const hostname)
{
    bool ok{};
    if (hostname != nullptr) {
        uint32_t addr0{INADDR_NONE};
        if (std::isdigit(hostname[0])) {
            // If 'hostname' starts with a number then first try to use it as an IP address
            addr0 = ::inet_addr(hostname);
            ok = (addr0 != INADDR_NONE);
        }
        if (addr0 == INADDR_NONE) {
            // Didn't work, try to find the host IP address by name
            if (isMessageEnabled(MSG_DEBUG)) {
                std::cout << "PosixHandler::setLocalAddr(): Looking up host name: " << hostname;
            }
            const hostent* p{gethostbyname(hostname)};
            if (p != nullptr && p->h_length > 0) {
                // 'q' points to the four byte address (in network order) as a single unsigned integer
                const unsigned int* const q{reinterpret_cast<const unsigned int*>(p->h_addr_list[0])};
                if (q != nullptr) {
                    struct in_addr in;
                    in.s_addr = *q;
                    addr0 = in.s_addr;

                    char* ipAddr{::inet_ntoa(in)};
                    if (ipAddr != nullptr) {
                        if (isMessageEnabled(MSG_DEBUG)) {
                           std::cout << " -- IP Address: " << ipAddr << std::endl;
                        }
                        ok = true;
                    }
                }
            }
            if (!ok && isMessageEnabled(MSG_DEBUG)) {
                std::cout << " -- HOST NOT FOUND!" << std::endl;
            }
        }
        if (addr0 != INADDR_NONE) ok = setLocalAddr(addr0);
    }
    return ok;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// localIpAddress: String containing the local IP address
bool IPosixHandler::setSlotLocalIpAddress(const Identifier* const x)
{
    localIpAddr = x->asString();
    return true;
}

// localIpAddress: String containing the local IP address
bool IPosixHandler::setSlotLocalIpAddress(const String* const x)
{
    localIpAddr = x->c_str();
    return true;
}

// port: Port number
bool IPosixHandler::setSlotPort(const Integer* const x)
{
    bool ok{};
    const int ii{x->asInt()};
    if (ii >= 0x0 && ii <= 0xffff) {
        ok = setPort( static_cast<uint16_t>(ii) );
    }
    return ok;
}

// localPort: Local (source) port number
bool IPosixHandler::setSlotLocalPort(const Integer* const x)
{
    bool ok{};
    const int ii{x->asInt()};
    if (ii >= 0x0 && ii <= 0xffff) {
        ok = setLocalPort( static_cast<uint16_t>(ii) );
    }
    return ok;
}

// shared: Reuse the port
bool IPosixHandler::setSlotShared(const Boolean* const x)
{
    setSharedFlag(x->asBool());
    return true;
}

// sendBuffSizeKb: Send buffer size in KB's    (default:  32 Kb)
bool IPosixHandler::setSlotSendBuffSize(const Integer* const x)
{
    bool ok{};
    const int ii{x->asInt()};
    if (ii >= 0 && ii <= 1024) {
        sendBuffSizeKb = ii;
        ok = true;
    }
    return ok;
}

// recvBuffSizeKb: Receive buffer size in KB's (default: 128 Kb)
bool IPosixHandler::setSlotRecvBuffSize(const Integer* const x)
{
    bool ok{};
    const int ii{x->asInt()};
    if (ii >= 0 && ii <= 1024) {
        recvBuffSizeKb = ii;
        ok = true;
    }
    return ok;
}

// setSlotIgnoreSourcePort: Ignore message from our this source port number
bool IPosixHandler::setSlotIgnoreSourcePort(const Integer* const x)
{
    bool ok{};
    const int ii{x->asInt()};
    if (ii >= 0x0 && ii <= 0xffff) {
        ignoreSourcePort = uint16_t(ii);
        ok = true;
    }
    return ok;
}

}
}
