
#ifndef __mixr_recorder_NetOutput_H__
#define __mixr_recorder_NetOutput_H__

#include "mixr/recorder/OutputHandler.hpp"

namespace mixr {
namespace base { class NetHandler; class Number; }
namespace recorder {

//------------------------------------------------------------------------------
// Class: NetOutput
// Description: Serialize and write a DataRecord to a network
//
// Factory name: NetOutput
// Slots:
//      netHandler  <NetHandler>    Network output handler
//      noWait      <Number>        No wait (unblocked) I/O flag (default: false -- blocked I/O)
//------------------------------------------------------------------------------
class NetOutput : public OutputHandler
{
    DECLARE_SUBCLASS(NetOutput, OutputHandler)

public:
   NetOutput();

   virtual bool isNetworkEnabled() const;    // Has the network been initialized?
   virtual bool initNetworks();              // Init the network
   virtual void closeConnections();          // close the network connection

protected:
   void processRecordImp(const DataRecordHandle* const handle) override;

private:
    base::safe_ptr<base::NetHandler> netHandler; // Network handler (input/output, or just output if netInput is defined)
    bool networkInitialized {};    // Network has been initialized
    bool networkInitFailed {};     // Network initialization has failed
    bool noWaitFlag {};            // No wait (unblocked) I/O flag

private:
   // slot table helper methods
   bool setSlotNetwork(base::NetHandler* const);
   bool setSlotNoWait(base::Number* const);
};

}
}

#endif
