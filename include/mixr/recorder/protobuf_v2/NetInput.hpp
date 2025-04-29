
#ifndef __mixr_recorder_NetInput_HPP__
#define __mixr_recorder_NetInput_HPP__

#include "mixr/recorder/IInputHandler.hpp"

namespace mixr {
namespace base { class Boolean; class INetHandler; }
namespace recorder {

//------------------------------------------------------------------------------
// Class: NetInput
// Description: Read and parse data records from a network stream
//
// Factory name: NetInput
// Slots:
//      netHandler  <INetHandler>   Network input handler
//      noWait      <Boolean>       No wait (unblocked) I/O flag (default: false -- blocked I/O)
//------------------------------------------------------------------------------
class NetInput : public IInputHandler
{
    DECLARE_SUBCLASS(NetInput, IInputHandler)

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

    base::safe_ptr<mixr::base::INetHandler> netHandler;   // Network handler (input/output, or just output if netInput is defined)
    bool networkInitialized {};        // Network has been initialized
    bool networkInitFailed {};         // Network initialization has failed
    bool noWaitFlag {};                // No wait (unblocked) I/O flag
    bool firstPassFlg {true};          // First pass flag

   char* ibuf {};    // Input buffer

private:
   // slot table helper methods
   bool setSlotNetwork(base::INetHandler* const);
   bool setSlotNoWait(base::Boolean* const);
};

}
}

#endif
