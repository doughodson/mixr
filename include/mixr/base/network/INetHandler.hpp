
#ifndef __mixr_base_INetHandler_HPP__
#define __mixr_base_INetHandler_HPP__

#include "mixr/base/IComponent.hpp"
#include "mixr/base/util/platform_api.hpp"
#include <cstdint>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: INetHandler
// Description: Interface that defines network handlers: Can be used for UDP/IP,
//              TCP/IP, Multicast and Broadcast.  Each handler manages a socket
//              and can be used to send data, receive data, or both.
//
// Windows: using Winsock2.h - link with Ws2_32.lib
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: INetHandler
// Slots: none
//------------------------------------------------------------------------------
class INetHandler : public IComponent
{
   DECLARE_SUBCLASS(INetHandler, IComponent)

public:  // Define the Windows vs Unix socket type
   #if defined(WIN32)
      typedef SOCKET LcSocket;
      static const LcSocket NET_INVALID_SOCKET{INVALID_SOCKET};
      static const int      NET_SOCKET_ERROR{SOCKET_ERROR};
   #else
      typedef int LcSocket;
      static const LcSocket NET_INVALID_SOCKET{-1};  // Always -1 and errno is
      static const int      NET_SOCKET_ERROR{-1};    // set
   #endif

public:
   INetHandler();

   // Initialize the network handler -- 'noWaitFlag' is true for unblocked I/O
   // Note: all parameters (slots) need to be set before calling this function.
   virtual bool initNetwork(const bool noWaitFlag);

   // Returns true if the network handler has been initialized (and connected if TCP)
   virtual bool isConnected() const =0;

   // Close (un-initialize) this network
   virtual bool closeConnection() =0;

   // Send 'size' bytes from packet; returns true if successful
   virtual bool sendData(const char* const packet, const int size) =0;

   // Receives a maximum of 'maxSize' bytes into 'packet.  Returns
   // the actual number of bytes received.
   virtual unsigned int recvData(char* const packet, const int maxSize) =0;

   // Set our socket for blocked (wait) I/O
   virtual bool setBlocked() =0;

   // Set our socket for unblocked (no wait) I/O
   virtual bool setNoWait() =0;

   // To/From NET byte swap routines
   static void toNet(const void* const hostData, void* const netData, const int nl, const int ns);
   static void toHost(const void* const netData, void* const hostData, const int nl, const int ns);

   // Byte order
   static bool isNetworkByteOrder()        { return netByteOrder; }
   static bool isNotNetworkByteOrder()     { return !netByteOrder; }
   static bool checkByteOrder();           // Returns true if in network byte order

   // Convert to network byte order ('vout' is network byte order from 'vin' host network order)
   static void toNetOrder(int16_t* const vout, const int16_t vin);
   static void toNetOrder(uint16_t* const vout, const uint16_t vin);
   static void toNetOrder(int32_t* const vout, const int32_t vin);
   static void toNetOrder(uint32_t* const vout, const uint32_t vin);
   static void toNetOrder(int64_t* const vout, const int64_t vin);
   static void toNetOrder(uint64_t* const vout, const uint64_t vin);
   static void toNetOrder(float* const vout, const float vin);
   static void toNetOrder(double* const vout, const double vin);

   // convert from network byte order ('vout' is host byte order from 'vin' network byte order)
   static void fromNetOrder(int16_t* const vout, const int16_t vin);
   static void fromNetOrder(uint16_t* const vout, const uint16_t vin);
   static void fromNetOrder(int32_t* const vout, const int32_t vin);
   static void fromNetOrder(uint32_t* const vout, const uint32_t vin);
   static void fromNetOrder(int64_t* const vout, const int64_t vin);
   static void fromNetOrder(uint64_t* const vout, const uint64_t vin);
   static void fromNetOrder(float* const vout, const float vin);
   static void fromNetOrder(double* const vout, const double vin);

protected:
   virtual bool init();            // Initialize this socket handler

private:
   static bool netByteOrder;       // True if this machine is in 'network byte order'
};

// ---
// Convert to network byte order
// ---
inline void INetHandler::toNetOrder(int16_t* const vout, const int16_t vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(int16_t);
        *--q = *p++;
        *--q = *p++;
    }
}

inline void INetHandler::toNetOrder(uint16_t* const vout, const uint16_t vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(uint16_t);
        *--q = *p++;
        *--q = *p++;
    }
}

inline void INetHandler::toNetOrder(int32_t* const vout, const int32_t vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(int32_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void INetHandler::toNetOrder(uint32_t* const vout, const uint32_t vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(uint32_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void INetHandler::toNetOrder(int64_t* const vout, const int64_t vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(int64_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void INetHandler::toNetOrder(uint64_t* const vout, const uint64_t vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(uint64_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void INetHandler::toNetOrder(float* const vout, const float vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(float);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void INetHandler::toNetOrder(double* const vout, const double vin)
{
    if (isNotNetworkByteOrder()) {
        auto p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&vin));
        auto q = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(vout)) + sizeof(double);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}


// ---
// convert from network byte order
// ---
inline void INetHandler::fromNetOrder(int16_t* const vout, const int16_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void INetHandler::fromNetOrder(uint16_t* const vout, const uint16_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void INetHandler::fromNetOrder(int32_t* const vout, const int32_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void INetHandler::fromNetOrder(uint32_t* const vout, const uint32_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void INetHandler::fromNetOrder(int64_t* const vout, const int64_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void INetHandler::fromNetOrder(uint64_t* const vout, const uint64_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void INetHandler::fromNetOrder(float* const vout, const float vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void INetHandler::fromNetOrder(double* const vout, const double vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

}
}

#endif

