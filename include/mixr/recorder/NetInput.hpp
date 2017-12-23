
#ifndef __mixr_recorder_NetInput_H__
#define __mixr_recorder_NetInput_H__

#include "mixr/recorder/InputHandler.hpp"

namespace mixr {
   namespace base { class NetHandler; class Number; }

namespace recorder {

//------------------------------------------------------------------------------
// Class: NetInput
// Description: Read and parse data records from a network stream
//
// Factory name: NetInput
// Slots:
//      netHandler  <NetHandler>    Network input handler
//      noWait      <Number>        No wait (unblocked) I/O flag (default: false -- blocked I/O)
//------------------------------------------------------------------------------
class NetInput : public InputHandler
{
    DECLARE_SUBCLASS(NetInput, InputHandler)

public:
   static const unsigned int MAX_INPUT_BUFFER_SIZE = 2000;

public:
   NetInput();

   virtual bool isNetworkEnabled() const;

   virtual bool initNetworks();
   virtual void closeConnections();

protected:

   const DataRecordHandle* readRecordImp() override;

private:
   void initData();

    base::safe_ptr<mixr::base::NetHandler> netHandler;   // Network handler (input/output, or just output if netInput is defined)
    bool networkInitialized {};        // Network has been initialized
    bool networkInitFailed {};         // Network initialization has failed
    bool noWaitFlag {};                // No wait (unblocked) I/O flag
    bool firstPassFlg {true};          // First pass flag

   char* ibuf {};    // Input buffer

private:
   // slot table helper methods
   bool setSlotNetwork(base::NetHandler* const);
   bool setSlotNoWait(base::Number* const);
};

}
}

#endif
