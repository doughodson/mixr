
#include "mixr/ighost/cigi/session/HostSession.hpp"

#include <memory>

#include "SignalProcessor.hpp"

#include "mixr/ighost/cigi/CigiHost.hpp"

#include "mixr/base/network/NetHandler.hpp"

#include "cigicl/CigiBaseSignalProcessing.h"

#include "cigicl/CigiEntityCtrlV3.h"
#include "cigicl/CigiCompCtrlV3.h"
#include "cigicl/CigiIGCtrlV3.h"
#include "cigicl/CigiLosVectReqV3.h"
#include "cigicl/CigiViewCtrlV3.h"
#include "cigicl/CigiViewDefV3.h"
#include "cigicl/CigiArtPartCtrlV3.h"
#include "cigicl/CigiHatHotReqV3.h"
#include "cigicl/CigiSOFV3.h"
#include "cigicl/CigiHatHotRespV3.h"
#include "cigicl/CigiHatHotRespV3_2.h"
#include "cigicl/CigiHatHotXRespV3_2.h"
#include "cigicl/CigiLosRespV3.h"
#include "cigicl/CigiCollDetSegRespV3.h"
#include "cigicl/CigiCollDetVolRespV3.h"
#include "cigicl/CigiSensorRespV3.h"
#include "cigicl/CigiIGMsgV3.h"
#include "cigicl/CigiHostSession.h"
#include "cigicl/CigiSensorCtrlV3.h"

namespace mixr {
namespace cigi {

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
static const int MAX_BUF_SIZE{1472};

IMPLEMENT_SUBCLASS(HostSession, "CigiHostSession")
EMPTY_DELETEDATA(HostSession)
EMPTY_COPYDATA(HostSession)

BEGIN_SLOTTABLE(HostSession)
   "netInput",             // 1) Network input handler
   "netOutput",            // 2) Network output handler
END_SLOTTABLE(HostSession)

BEGIN_SLOT_MAP(HostSession)
   ON_SLOT(1, setSlotNetInput,  base::NetHandler)
   ON_SLOT(2, setSlotNetOutput, base::NetHandler)
END_SLOT_MAP()

HostSession::HostSession()
{
   STANDARD_CONSTRUCTOR()
}

// initialize session, signal callback interface and network connections
bool HostSession::initialize(CigiHost* const p)
{
   // our parent component
   parent = p;

   // create cigi session
   session.reset(new CigiHostSession(4, MAX_BUF_SIZE, 4, MAX_BUF_SIZE));
   if (session == nullptr) return false;

   //session->SetCigiVersion(2);     // CGB only force this if necessary
   // CGBCGB ??? session->SetSynchronous(p->isSyncMode());

   // setup convenience pointers to message queues
   msgOut = &session->GetOutgoingMsgMgr();
   msgIn = &session->GetIncomingMsgMgr();

   // create signal processor object
   sigProcessor.reset(new SignalProcessor(parent));

   msgIn->SetReaderCigiVersion(2);
   msgIn->UsingIteration(false);
   msgIn->RegisterSignalProcessor(sigProcessor.get());

   // initialize network connections
   if (!networkInitialized && !networkInitFailed) {
      networkInitialized = initNetworkConnections();
      networkInitFailed = !networkInitialized;
      if (networkInitFailed) return false;
   }

   if (isMessageEnabled(MSG_INFO)) {
      std::cout << "HostSession::initialize(): CIGI host session initialized!" << std::endl;
   }
   return true;
}

// returns status of this host session manager
// if session created, networks initialized ok, and we thread started, we are good
bool HostSession::isInitialized()
{
   return (session != nullptr) && networkInitialized;
}

// initialize network connections
bool HostSession::initNetworkConnections()
{
   bool ok{true};

   // initialize network input handler
   if (netInput != nullptr) {
      if (netInput->initNetwork(false)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "netInput initialized OK" << std::endl;
         }
      }
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "HostSession::initNetworkConnections(): failure to find the network input handler (see slot 'netInput')" << std::endl;
      }
      ok = false;
   }

   // initialize network output handler
   if (netOutput != nullptr) {
      if (netOutput->initNetwork(true)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "netOutput initialized OK" << std::endl;
         }
      }
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "HostSession::initNetworkConnections(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
      }
      ok = false;
   }

   return ok;
}

//------------------------------------------------------------------------------
// CIGI functions
//------------------------------------------------------------------------------

// start the message.  This MUST come before any CigiAddPacket*() functions.
void HostSession::startMessage()
{
   msgOut->BeginMsg();
}

// end the message
void HostSession::endMessage()
{
#if 1
   msgOut->UpdateFrameCntr();
   msgOut->LockMsg();

   int sendSize{};
   unsigned char* sendBuff{msgOut->GetMsg(sendSize)};
   netOutput->sendData( reinterpret_cast<char*>(sendBuff), sendSize );

   msgOut->UnlockMsg();
#else
   // CGB this is sync mode, and I think it only works for version 3 in the class lib right now
   msgOut->LockMsg();

   int sendSize{};
   unsigned char* sendBuff{msgOut->GetMsg(sendSize)};
   unsigned char* recvBuff{msgIn->GetMsgBuffer()};

   msgOut->UpdateFrameCntr(sendBuff, recvBuff);

   netOutput->sendData( reinterpret_cast<char*>(sendBuff), sendSize );

   msgOut->UnlockMsg();
#endif
}

int HostSession::getOutgoingBufferSize()
{
   int sendSize{};
   msgOut->GetMsg(sendSize);
   return sendSize;
}

// add a IG Control packet
void HostSession::addPacketIGCtrl(CigiIGCtrlV3* const p)
{
   *msgOut << *p;
}

// add a View Control packet
void HostSession::addPacketSensorCtrl(CigiSensorCtrlV3* const p)
{
   *msgOut << *p;
}

// add a View Control packet
void HostSession::addPacketViewCtrl(CigiViewCtrlV3* const p)
{
   *msgOut << *p;
}

// add a View Definition packet
void HostSession::addPacketViewDef(CigiViewDefV3* const p)
{
   *msgOut << *p;
}

// add an entity control packet
void HostSession::addPacketEntityCtrl(CigiEntityCtrlV3* const p)
{
   *msgOut << *p;
}

// add a Component Control packet
void HostSession::addPacketComponentCtrl(CigiCompCtrlV3* const p)
{
   *msgOut << *p;
}

// add a Articulated Part Control packet
void HostSession::addPacketArtPartCtrl(CigiArtPartCtrlV3* const p)
{
   *msgOut << *p;
}

// add a Height Of Terrain (HOT) Request packet
void HostSession::addPacketHatHotReq(CigiHatHotReqV3* const p)
{
   *msgOut << *p;
}

// add a LOS/Range Request packet
void HostSession::addPacketLosRangeReq(CigiLosVectReqV3* const p)
{
   *msgOut << *p;
}

// process IG messages
void HostSession::processIgMessages()
{
   // receive and process incoming IG CIGI messages
   if (netInput != nullptr && netOutput != nullptr) {
      while ( !parent->isShutdown() ) {

         msgIn->AdvanceCrntBuffer();
         unsigned char* rcvbuff{msgIn->GetMsgBuffer()};
         int recvCnt{static_cast<int>(netInput->recvData(reinterpret_cast<char*>(rcvbuff), MAX_BUF_SIZE))};

         if (recvCnt > 0) {
            msgIn->SetCrntMsgSize(recvCnt);
            msgIn->ProcessIncomingMsg();
         } else {
            msgIn->SetCrntMsgSize(0);
         }
      }
   }
}

bool HostSession::setSlotNetInput(base::NetHandler* const msg)
{
   netInput = msg;
   return true;
}

bool HostSession::setSlotNetOutput(base::NetHandler* const msg)
{
   netOutput = msg;
   return true;
}

}
}
