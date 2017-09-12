
#ifndef __mixr_ig_cigi_CigiClNetwork_H__
#define __mixr_ig_cigi_CigiClNetwork_H__

#include "mixr/ig/cigi/AbstractCigiCl.hpp"

class CigiIGCtrlV3;
class CigiViewCtrlV3;
class CigiSensorCtrlV3;
class CigiViewDefV3;
class CigiEntityCtrlV3;
class CigiCompCtrlV3;
class CigiArtPartCtrlV3;
class CigiHatHotRespV3;
class CigiLosVectReqV3;   // CGBCGB CIGI_LOS_RANGE_REQUEST* los;          // LOS request packet

class CigiIncomingMsg;
class CigiOutgoingMsg;

namespace mixr {
namespace base { class NetHandler; class Thread; }
namespace cigi {
class OtwCigiCl;
class CigiClNetworkSignalProcessing;

//------------------------------------------------------------------------------
// Class: CigiClNetwork
//
// Description: Networked CIGI interface to the IG system
//
// Factory name: CigiClNetwork
// Slots:
//    netInput       (NetHandler)   Network input handler
//    netOutput      (NetHandler)   Network output handler
//------------------------------------------------------------------------------
class CigiClNetwork : public AbstractCigiCl
{
   DECLARE_SUBCLASS(CigiClNetwork, AbstractCigiCl)

public:
   CigiClNetwork();

   // get a pre-ref'd pointer to the network input handler
   virtual base::NetHandler* getInputHandler();

   // get a pre-ref'd pointer to the network output handler
   virtual base::NetHandler* getOutputHandler();

   // CIGI's (sync-mode) main network loop
   virtual void mainLoop();

   virtual bool initialize(OtwCigiCl* const) override;
   virtual bool isInitialized() override;
   virtual void startMessage() override;
   virtual void endMessage() override;
   virtual int  getOutgoingBufferSize() override;

   virtual void addPacketIGCtrl(CigiIGCtrlV3* const) override;
   virtual void addPacketViewCtrl(CigiViewCtrlV3* const) override;
   virtual void addPacketSensorCtrl(CigiSensorCtrlV3* const) override;
   virtual void addPacketViewDef(CigiViewDefV3* const) override;
   virtual void addPacketEntityCtrl(CigiEntityCtrlV3* const) override;
   virtual void addPacketComponentCtrl(CigiCompCtrlV3* const) override;
   virtual void addPacketArtPartCtrl(CigiArtPartCtrlV3* const) override;
   virtual void addPacketHatHotReq(CigiHatHotReqV3* const) override;
   virtual void addPacketLosRangeReq(CigiLosVectReqV3* const) override;

   CigiIncomingMsg* getCigiIncomingMsg() { return msgIn; };
   CigiOutgoingMsg* getCigiOutgoingMsg() { return msgOut; };

protected:
   bool createCigiProcess();        // Create the CIGI network thread
   bool initCigiNetwork();          // Initialize the network

   CigiIncomingMsg* msgIn {};
   CigiOutgoingMsg* msgOut {};
   CigiClNetworkSignalProcessing* sigProcessor {};

private:
   base::safe_ptr<base::NetHandler> netInput;   // Input network handler
   base::safe_ptr<base::NetHandler> netOutput;  // Output network handler
   base::safe_ptr<base::Thread> thread;         // The thread
   bool networkInitialized {};                  // CIGI has been initialized
   bool networkInitFailed {};                   // CIGI initialization has failed

private:
   // slot table helper methods
   bool setSlotNetInput(base::NetHandler* const);
   bool setSlotNetOutput(base::NetHandler* const);
};

}
}

#endif
