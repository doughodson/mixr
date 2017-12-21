
#ifndef __mixr_ighost_cigi_session_HostSession_H__
#define __mixr_ighost_cigi_session_HostSession_H__

#include "mixr/base/Component.hpp"

#include <memory>

class CigiHostSession;

class CigiIGCtrlV3;
class CigiViewCtrlV3;
class CigiSensorCtrlV3;
class CigiViewDefV3;
class CigiEntityCtrlV3;
class CigiCompCtrlV3;
class CigiArtPartCtrlV3;
class CigiHatHotReqV3;
class CigiHatHotRespV3;
class CigiLosVectReqV3;   // CGBCGB CIGI_LOS_RANGE_REQUEST* los;          // LOS request packet

class CigiIncomingMsg;
class CigiOutgoingMsg;

namespace mixr {
namespace base { class NetHandler; }
namespace cigi {
class CigiHost;
class SignalProcessor;

//------------------------------------------------------------------------------
// Class: HostSession
//
// Description: Manages a networked CIGI session from host (simulation)
//              to IG system.  This object establishes a CIGI session,
//              a signal processor and establishes network connections
//              to receive and process messages from IG system.
//
// Factory name: CigiHostSession
// Slots:
//    netInput       (NetHandler)   Network input handler
//    netOutput      (NetHandler)   Network output handler
//------------------------------------------------------------------------------
class HostSession : public base::Component
{
   DECLARE_SUBCLASS(HostSession, base::Component)

public:
   HostSession();

   void processIgMessages();

   bool initialize(CigiHost* const);
   bool isInitialized();

   void startMessage();
   void endMessage();
   int getOutgoingBufferSize();

   void addPacketIGCtrl(CigiIGCtrlV3* const);
   void addPacketViewCtrl(CigiViewCtrlV3* const);
   void addPacketSensorCtrl(CigiSensorCtrlV3* const);
   void addPacketViewDef(CigiViewDefV3* const);
   void addPacketEntityCtrl(CigiEntityCtrlV3* const);
   void addPacketComponentCtrl(CigiCompCtrlV3* const);
   void addPacketArtPartCtrl(CigiArtPartCtrlV3* const);
   void addPacketHatHotReq(CigiHatHotReqV3* const);
   void addPacketLosRangeReq(CigiLosVectReqV3* const);

private:
   CigiHost* parent{};   // parent (host) component

   // cigi session, incoming and outgoing message pointers
   std::unique_ptr<CigiHostSession> session;
   CigiIncomingMsg* msgIn{};
   CigiOutgoingMsg* msgOut{};

   bool initNetworkConnections();

   base::safe_ptr<base::NetHandler> netInput;   // input network handler
   base::safe_ptr<base::NetHandler> netOutput;  // output network handler

   bool networkInitialized{};                   // CIGI has been initialized
   bool networkInitFailed{};                    // CIGI initialization has failed

   std::unique_ptr<SignalProcessor> sigProcessor;

private:
   // slot table helper methods
   bool setSlotNetInput(base::NetHandler* const);
   bool setSlotNetOutput(base::NetHandler* const);
};

}
}

#endif
