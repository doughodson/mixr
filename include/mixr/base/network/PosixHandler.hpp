
#ifndef __mixr_base_PosixHandler_HPP__
#define __mixr_base_PosixHandler_HPP__

#include "mixr/base/network/INetHandler.hpp"

#include <string>

namespace mixr {
namespace base {
class Boolean;
class Identifier;
class Integer;
class String;

//------------------------------------------------------------------------------
// Class: PosixHandler
// Description: Abstract (connectionless) network handler that include Posix
//              oriented methods to support UDP/IP, TCP/IP, Multicast and Broadcast.
//              Each handler manages a socket and can be used to send data, receive
//              data, or both.
//
// Windows: using Winsock2.h; link with Ws2_32.lib
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: AbstractPosixHandler
// Slots:
//    localIpAddress    ! IP address in Internet standard "." (dotted) notation
//                      ! written as a string "127.0.0.1"
//    localIpAddress    ! IP address name identifier (e.g., 'localhost') to be
//                      ! used to look up a particular address
//
//    localPort         ! Local (source) port number (optional) (see examples below)
//                      !   send:    Local port that we'll send the packets from
//                      !   receive: Local port that we'll read packets
//
//    port              ! Number of the port we're connecting or listening to (required) (see examples below)
//                      !   send:    Report port we'll send packets to, and if 'localPort' is
//                      !            not defined, it's the local port we'll send data from.
//                      !   receive: We'll receive packets from this port if 'localPort'
//                      !            not defined.
//
//    shared            ! Set socket's shared (reuse) flag [default: false (not shared)]
//
//    sendBuffSizeKb    ! Send buffer size in KB's    (default:  32 Kb; max 1024)
//
//    recvBuffSizeKb    ! Receive buffer size in KB's (default: 128 Kb; max 1024)
//
//    ignoreSourcePort  ! Ignore messages from this port. This is one way to prevent receiving our
//                      ! own data with multicast or broadcast.
//
//
//    Local host side   Data Flow   Remote host side
//    ---------------   ---------   ----------------
//
//      port#           ------->       port#       ! Sending with only 'port' defined
//
//      localPort#      ------->       port#       ! Sending with 'port' and 'localPort' defined
//
//      port#           <-------     <any-port>    ! Receiving with only port defined
//
//      localPort#      <-------     <any-port>    ! Receiving anytime that 'localPort' is defined.
//
//------------------------------------------------------------------------------
// Notes:
//
// M$ WinSock has slightly different return types, some different calling, and
// is missing some of the calls that are standard in Berkeley and POSIX socket
// implementation.  These slight differences will be handled in setting basic
// typedefs, defines, and constants that will make each convention match for
// use later in the code.  This will save a lot of pre-processor intervention
// and make the code that much more enjoyable to read!
//------------------------------------------------------------------------------
class PosixHandler : public INetHandler
{
   DECLARE_SUBCLASS(PosixHandler, INetHandler)

public:
   PosixHandler();

   uint16_t getPort() const;                    // Port#
   uint16_t getLocalPort() const;               // Local port #
   uint16_t getIgnoreSourcePort() const;        // Ignore source port #

   uint32_t getLocalAddr() const;               // Gets the local host IP address
   uint32_t getNetAddr() const;                 // Gets the network (remote) host IP address

   bool getSharedFlag() const;                  // Is the socket address shared?
   void setSharedFlag(const bool);

   bool initNetwork(const bool noWaitFlag) override;
   bool isConnected() const override;
   bool closeConnection() override;
   bool sendData(const char* const packet, const int size) override;
   unsigned int recvData(char* const packet, const int maxSize) override;
   bool setBlocked() override;
   bool setNoWait() override;

   // Last recvData() origin IP and port
   uint32_t getLastFromAddr() const;     // IP address of last valid recvData()
   uint16_t getLastFromPort() const;     // Port address of last valid recvData()

protected:
   bool init() override;

   virtual bool bindSocket();          // Bind socket to address

   // Sets the network IP address
   bool setNetAddr(const uint32_t);

   // Sets the network IP address by host name
   bool setNetAddr(const char* const hostname);

   // Sets the port number
   bool setPort(const uint16_t);

   // Sets the local IP address
   bool setLocalAddr(const uint32_t);

   // Sets the local IP address using the Internet standard "." (dotted) notation
   bool setLocalAddr(const char* const);

   // Sets the local port number
   bool setLocalPort(const uint16_t);

   // Set the output buffer size
   bool setSendBuffSize();

   // Sets the input buffer size
   bool setRecvBuffSize();

   LcSocket socketNum;                // uur Socket

private:
   std::string localIpAddr;           // Local host name or IP address
   uint32_t localAddr{};              // Local host address
   uint32_t netAddr{};                // Network (remote) host address
   uint32_t fromAddr1{};              // Last recvData() 'from' ip address
   uint16_t port{};                   // Port
   uint16_t localPort{};              // Local (source) port
   uint16_t ignoreSourcePort{};       // Ignore message from this source port
   uint16_t fromPort1{};              // Last recvData() 'from' port number
   bool sharedFlg{};                  // Shared port flag
   bool initialized{};                // handler has been initialized
   unsigned int sendBuffSizeKb{32};   // Send buffer size in KBs
   unsigned int recvBuffSizeKb{128};  // Receive buffer size in KBs

private:
   // slot table helper methods
   bool setSlotLocalIpAddress(const String* const);
   bool setSlotLocalIpAddress(const Identifier* const);
   bool setSlotPort(const Integer* const);
   bool setSlotLocalPort(const Integer* const);
   bool setSlotShared(const Boolean* const);
   bool setSlotSendBuffSize(const Integer* const);
   bool setSlotRecvBuffSize(const Integer* const);
   bool setSlotIgnoreSourcePort(const Integer* const);
};

// Port#
inline uint16_t PosixHandler::getPort() const
{
   return port;
}

// Local port #
inline uint16_t PosixHandler::getLocalPort() const
{
   return localPort;
}

// Ignore source port #
inline uint16_t PosixHandler::getIgnoreSourcePort() const
{
   return ignoreSourcePort;
}

// Gets the local host IP address
inline uint32_t PosixHandler::getLocalAddr() const
{
   return localAddr;
}

// Gets the network (remote) host IP address
inline uint32_t PosixHandler::getNetAddr() const
{
   return netAddr;
}

// Shared flag
inline bool PosixHandler::getSharedFlag() const
{
   return sharedFlg;
}

inline uint32_t PosixHandler::getLastFromAddr() const
{
   return fromAddr1;
}

inline uint16_t PosixHandler::getLastFromPort() const
{
   return fromPort1;
}

}
}

#endif

