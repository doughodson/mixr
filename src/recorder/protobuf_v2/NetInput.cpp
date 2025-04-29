
#include "mixr/recorder/protobuf_v2/NetInput.hpp"
#include "mixr/recorder/protobuf_v2/proto/DataRecord.pb.h"
#include "mixr/recorder/protobuf_v2/DataRecordHandle.hpp"
#include "mixr/base/network/INetHandler.hpp"
#include "mixr/base/numeric/Boolean.hpp"

namespace mixr {
namespace recorder {

IMPLEMENT_SUBCLASS(NetInput, "RecorderNetInput")

BEGIN_SLOTTABLE(NetInput)
   "netHandler",           // 1) Network handler
   "noWait",               // 2) No wait (unblocked) I/O flag (default: false -- blocked I/O)
END_SLOTTABLE(NetInput)

BEGIN_SLOT_MAP(NetInput)
    ON_SLOT(1, setSlotNetwork,   mixr::base::INetHandler)
    ON_SLOT(2, setSlotNoWait,    mixr::base::Boolean)
END_SLOT_MAP()

NetInput::NetInput()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void NetInput::initData()
{
   ibuf = new char[MAX_INPUT_BUFFER_SIZE];
}

void NetInput::copyData(const NetInput& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   noWaitFlag = org.noWaitFlag;

   // We need to init this ourselves, so ...
   netHandler = nullptr;
   networkInitialized = false;
   networkInitFailed = false;
   firstPassFlg = true;
}

void NetInput::deleteData()
{
   closeConnections();
   netHandler = nullptr;
   if (ibuf != nullptr) { delete[] ibuf; ibuf = nullptr; }
}


//------------------------------------------------------------------------------
// Returns true if the networks are initialized and connected
//------------------------------------------------------------------------------
bool NetInput::isNetworkEnabled() const
{
   bool ok{networkInitialized && netHandler->isConnected()};
   return ok;
}


//------------------------------------------------------------------------------
// Init the network
//------------------------------------------------------------------------------
bool NetInput::initNetworks()
{
   bool ok{};
   if (netHandler != nullptr) {
      ok = netHandler->initNetwork(noWaitFlag);
      networkInitialized = ok;
      networkInitFailed = !ok;
   }
   return ok;
}

//------------------------------------------------------------------------------
// close the network connection
//------------------------------------------------------------------------------
void NetInput::closeConnections()
{
   if (netHandler != nullptr && networkInitialized) netHandler->closeConnection();
   networkInitialized = false;
   networkInitFailed = false;
}

//------------------------------------------------------------------------------
// Read a record
//------------------------------------------------------------------------------
const DataRecordHandle* NetInput::readRecordImp()
{
   // First pass?  Does the file need to be opened?
   if (firstPassFlg) {
      if ( !networkInitialized ) {
         initNetworks();
      }
      firstPassFlg = false;
   }

   DataRecordHandle* handle = nullptr;

   // When the file is open and ready ...
   if ( networkInitialized && netHandler->isConnected() ) {

      // ---
      // Try to read a message into 'ibuf'
      // ---
      unsigned int n{netHandler->recvData( ibuf, MAX_INPUT_BUFFER_SIZE )};

      // ---
      // If we've successfully read a message from the network
      // then parse it as a DataRecord and put it into a Handle.
      // ---
      if (n > 0) {
         // Parse the data record
         std::string wireFormat(ibuf, n);
         auto dataRecord = new pb::DataRecord();
         bool ok{dataRecord->ParseFromString(wireFormat)};

         if (ok) {
            // Create a handle for the data record (it now has ownership)
            handle = new DataRecordHandle(dataRecord);
         }

         else if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
            std::cerr << "NetInput::readRecord() -- ParseFromString() error" << std::endl;
            delete dataRecord;
            dataRecord = nullptr;
         }

      }
   }
   return handle;
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Network Handler
bool NetInput::setSlotNetwork(mixr::base::INetHandler* const msg)
{
   netHandler = msg;
   return true;
}

// No wait (unblocked) I/O flag
bool NetInput::setSlotNoWait(mixr::base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      noWaitFlag = msg->asBool();
      ok = true;
   }
   return ok;
}

}
}
