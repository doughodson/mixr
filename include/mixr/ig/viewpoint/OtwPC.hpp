
#ifndef __mixr_ig_viewpoint_OtwPC_H__
#define __mixr_ig_viewpoint_OtwPC_H__

#include "mixr/ig/common/Otw.hpp"

namespace mixr {
namespace base { class NetHandler; }
namespace otw { class OtwModel; }
namespace viewpoint {

//------------------------------------------------------------------------------
// Class: OtwPC
//------------------------------------------------------------------------------
class OtwPC : public ig::Otw
{
   DECLARE_SUBCLASS(OtwPC, ig::Otw)

public:
   OtwPC();

   virtual void reset() override;                          // Send frame sync (if any)

protected:
   virtual void sendOwnshipAndModels() override;           // Send state data for ownship and models
   virtual void sendElevationRequests() override;          // Sends terrain height requests
   virtual void recvElevations() override;                 // Receives terrain height data
   virtual void frameSync() override;                      // Send frame sync (if any)
   virtual ig::OtwModel* modelFactory() override;         // Create OtwModel objects unique to interface
   virtual ig::OtwModel* hotFactory() override;           // Create OtwHot objects unique to interface

   bool isNetworkInitialized() const       { return netInit; }
   bool didInitializationFail() const      { return netInitFail; }

private:
   void sendPcData();                      // Send PC data block
   bool initNetwork();                     // Initialize the network

   base::safe_ptr<base::NetHandler> netOutput;  // Output network handler
   bool netInit {};                             // Network has been initialized
   bool netInitFail {};                         // Initialization attempt failed
   int scnt {};                                 // Send timer

private:
   // slot table helper methods
   bool setSlotNetOutput(base::NetHandler* const);
};

}
}

#endif
