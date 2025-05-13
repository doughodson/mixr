
#ifndef __mixr_models_common_Datalink_HPP__
#define __mixr_models_common_Datalink_HPP__

#include "mixr/models/system/ISystem.hpp"

#include "mixr/base/safe_queue.hpp"

namespace mixr {
namespace base { class Integer; class ILength; class String; }
namespace models {
class CommRadio;
class ITrackMgr;

//------------------------------------------------------------------------------
// Class: Datalink
// Description: Base class for all datalink models
//
// Factory name: Datalink
// Slots:
//    radioId           <Integer>    ! Radio ID (see note #1) (default: 0)
//    maxRange          <ILength>    ! Max range of the datalink (w/o a radio model) (see note #2)
//                                   ! (default: 5000)
//    radioName         <Identifier> ! Name of the (optional) communication radio model (see notes #1 and #2)
//                                   ! (default: 0)
//    trackManagerName  <Identifier> ! Track Manager Name (default: 0)
//
// Events:
//    DATALINK_MESSAGE  (base::Object)  Default handler: Pass messages to subcomponents.
//
//  Notes:
//    1) If both 'radioId' and 'radioName' are given and 'radioId' is not zero
//       then 'radioId' will override the ID of the named radio.
//    2) 'maxRange' is used when a named radio, 'radioName', is not provided.
//    3) This class is one of the "top level" systems attached to a Player
//       class (see Player.hpp).
//------------------------------------------------------------------------------
class Datalink : public ISystem
{
   DECLARE_SUBCLASS(Datalink, ISystem)

public:
   Datalink();

   virtual bool sendMessage(base::IObject* const msg);
   virtual base::IObject* receiveMessage();

   unsigned short getRadioID() const;

   CommRadio* getRadio()                                               { return radio; }
   const CommRadio* getRadio() const                                   { return radio; }
   virtual bool setRadio(CommRadio* const);

   const base::String* getRadioName() const                            { return radioName; }
   bool setRadioName(const base::String* const);

   // Max range (nm) of datalink without radio
   double getMaxRange() const                                          { return noRadioMaxRange; }
   virtual bool setMaxRange(const double nm);

   // Send messages to our local players; direct or via the radio (default: true)
   bool isLocalSendEnabled() const                                     { return sendLocal; }
   virtual bool setLocalSendEnabled(const bool flg);

   // Send messages to the network output queue (default: true)
   bool isNetworkQueueEnabled() const                                  { return queueForNetwork; }
   virtual bool setNetworkQueueEnabled(const bool flg);

   // For network handler to get to the messages
   base::safe_queue<base::IObject*>* getOutputQueue()                  { return outQueue; }

   ITrackMgr* getTrackManager()                                        { return trackManager; }
   const ITrackMgr* getTrackManager() const                            { return trackManager; }
   virtual bool setTrackManager(ITrackMgr* const tm);

   const base::String* getTrackManagerName() const                     { return tmName; }
   bool setTrackManagerName(const base::String* const);

   // Event handler(s)
   virtual bool onDatalinkMessageEvent(base::IObject* const);

   bool event(const int event, base::IObject* const obj = nullptr) override;
   void reset() override;

protected:
   virtual bool queueIncomingMessage(base::IObject* const);  // Queue up an incoming message
   virtual bool queueOutgoingMessage(base::IObject* const);  // Queue up an outgoing message
   virtual void clearQueues(); // Clear all queues

   void dynamics(const double dt) override;

   bool shutdownNotification() override;

private:
   void initData();

   static const int MAX_MESSAGES{1000};    // Max number of messages in queues

   base::safe_queue<base::IObject*>* inQueue {};  // Received message queue
   base::safe_queue<base::IObject*>* outQueue {}; // Queue for messages going out over the network/DIS
   double noRadioMaxRange {5000.0};               // Max range of our datalink (NM)

   const base::String* radioName {};     // Name of our radio
   CommRadio* radio {};                  // Our radio

   unsigned short radioId {};            // Radio ID input
   bool useRadioIdFlg {};                // Use the 'radioId' flag

   bool sendLocal {true};                // Send to local players flag; direct or via the radio
   bool queueForNetwork {true};          // Send the message to the network output queue

   ITrackMgr* trackManager {};           // Track manager
   const base::String* tmName {};        // Track manager name

private:
   // slot table helper methods
   bool setSlotRadioId(const base::Integer* const);
   bool setSlotMaxRange(const base::ILength* const);
   bool setSlotRadioName(const base::String* const x)                { return setRadioName(x); }
   bool setSlotTrackManagerName(const base::String* const x)         { return setTrackManagerName(x); }
};

}
}

#endif
