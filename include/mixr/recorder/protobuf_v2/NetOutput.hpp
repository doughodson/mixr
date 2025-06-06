
#ifndef __mixr_recorder_NetOutput_HPP__
#define __mixr_recorder_NetOutput_HPP__

#include "mixr/recorder/protobuf_v2/OutputHandler.hpp"

namespace mixr {
namespace base { class Boolean; class INetHandler; }
namespace recorder {
namespace protobuf_v2 {

//------------------------------------------------------------------------------
// Class: NetOutput
// Description: Serialize and write a DataRecord to a network
//
// Factory name: NetOutput
// Slots:
//      netHandler  <INetHandler>  ! Network output handler
//      noWait      <Boolean>      ! No wait (unblocked) I/O flag (default: false -- blocked I/O)
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
    base::safe_ptr<base::INetHandler> netHandler; // Network handler (input/output, or just output if netInput is defined)
    bool networkInitialized {};    // Network has been initialized
    bool networkInitFailed {};     // Network initialization has failed
    bool noWaitFlag {};            // No wait (unblocked) I/O flag

private:
   // slot table helper methods
   bool setSlotNetwork(base::INetHandler* const);
   bool setSlotNoWait(base::Boolean* const);
};

}
}
}

#endif
