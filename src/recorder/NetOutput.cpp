
#include "openeaagles/recorder/NetOutput.hpp"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"
#include "openeaagles/recorder/DataRecordHandle.hpp"
#include "openeaagles/base/network/NetHandler.hpp"
#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace recorder {

IMPLEMENT_SUBCLASS(NetOutput, "RecorderNetOutput")
EMPTY_SERIALIZER(NetOutput)

BEGIN_SLOTTABLE(NetOutput)
   "netHandler",           // 1) Network handler
   "noWait",               // 2) No wait (unblocked) I/O flag (default: false -- blocked I/O)
END_SLOTTABLE(NetOutput)

// Map slot table to handles
BEGIN_SLOT_MAP(NetOutput)
    ON_SLOT(1, setSlotNetwork,   oe::base::NetHandler)
    ON_SLOT(2, setSlotNoWait,    oe::base::Number)
END_SLOT_MAP()

NetOutput::NetOutput() : netHandler(nullptr)
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void NetOutput::initData()
{
   networkInitialized = false;
   networkInitFailed = false;
   noWaitFlag = false;
}

void NetOutput::copyData(const NetOutput& org, const bool)
{
   BaseClass::copyData(org);

   // We need to init this ourselves, so ...
   netHandler = nullptr;
   networkInitialized = false;
   networkInitFailed = false;
}

void NetOutput::deleteData()
{
   closeConnections();
   netHandler = nullptr;
}


//------------------------------------------------------------------------------
// Returns true if the networks are initialized and connected
//------------------------------------------------------------------------------
bool NetOutput::isNetworkEnabled() const
{
   bool ok = networkInitialized && netHandler->isConnected();
   return ok;
}


//------------------------------------------------------------------------------
// Init the network
//------------------------------------------------------------------------------
bool NetOutput::initNetworks()
{
   bool ok = false;
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
void NetOutput::closeConnections()
{
   if (netHandler != nullptr && networkInitialized) netHandler->closeConnection();
   networkInitialized = false;
   networkInitFailed = false;
}


//------------------------------------------------------------------------------
// Serialize and write a DataRecord to a network
//------------------------------------------------------------------------------
void NetOutput::processRecordImp(const DataRecordHandle* const handle)
{
   bool thisIsEodMsg = false;

   // ---
   // Open the file, if it hasn't been already ...
   // ---
   if ( !networkInitialized && !networkInitFailed ) initNetworks();

   if (handle != nullptr && networkInitialized && netHandler->isConnected()) {

      // The DataRecord to be sent
      const pb::DataRecord* dataRecord = handle->getRecord();

      // Serialize the DataRecord
      std::string wireFormat;
      bool ok = dataRecord->SerializeToString(&wireFormat);

      // Write the serialized message to the network
      if (ok) {
         netHandler->sendData( wireFormat.c_str(), wireFormat.length() );
      }

      else if (isMessageEnabled(MSG_ERROR | MSG_WARNING)) {
         // If we had an error serializing the DataRecord
         std::cerr << "NetOutput::processRecordImp() -- SerializeToString() error" << std::endl;
      }

      // Check for END_OF_DATA message
      thisIsEodMsg = (dataRecord->id() == REID_END_OF_DATA);

   }

   // ---
   // Close the file at END_OF_DATA message
   // ---
   if (thisIsEodMsg) {
      closeConnections();
   }
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Network Handler
bool NetOutput::setSlotNetwork(oe::base::NetHandler* const msg)
{
   netHandler = msg;
   return true;
}

// No wait (unblocked) I/O flag
bool NetOutput::setSlotNoWait(oe::base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      noWaitFlag = msg->getBoolean();
      ok = true;
   }
   return ok;
}

}
}
