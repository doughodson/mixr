
#include "mixr/recorder/protobuf_v2/OutputHandler.hpp"
#include "mixr/recorder/protobuf_v2/DataRecordHandle.hpp"
#include "mixr/recorder/protobuf_v2/proto/DataRecord.pb.h"

#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace recorder {
namespace protobuf_v2 {

IMPLEMENT_SUBCLASS(OutputHandler, "OutputHandler")
EMPTY_SLOTTABLE(OutputHandler)

OutputHandler::OutputHandler()
{
   STANDARD_CONSTRUCTOR()
}

void OutputHandler::copyData(const OutputHandler& org, const bool)
{
   BaseClass::copyData(org);

   // Don't copy the queue
   base::lock(semaphore);
   queue.clear();
   base::unlock(semaphore);
}

void OutputHandler::deleteData()
{
   // clear the queue
   base::lock(semaphore);
   queue.clear();
   base::unlock(semaphore);
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool OutputHandler::shutdownNotification()
{
   // Pass the shutdown notification to our subcomponent recorders
   base::PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
      for (base::IList::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         OutputHandler* sc{static_cast<OutputHandler*>(pair->object())};
         sc->event(SHUTDOWN_EVENT);
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }

   return BaseClass::shutdownNotification();
}


//------------------------------------------------------------------------------
// Pass the data record to all of our subcomponents for processing; they all
// should be of type OutputHandler (see processComponents() above)
//------------------------------------------------------------------------------
void OutputHandler::processRecord(const DataRecordHandle* const dataRecord)
{
   // Check the data filters to see if we should process this type record
   if ( isDataTypeEnabled(dataRecord) ) {

      // First, call our own implementation
      processRecordImp(dataRecord);

      // Next, pass the data record to our subcomponent OutputHandlers
      // for further processing
      base::PairStream* subcomponents{getComponents()};
      if (subcomponents != nullptr) {
         for (base::IList::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {

            base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            OutputHandler* sc{static_cast<OutputHandler*>(pair->object())};

            sc->processRecord(dataRecord);
         }
         subcomponents->unref();
         subcomponents = nullptr;
      }

   }
}


//------------------------------------------------------------------------------
// Queue the data record handle to be processed later
//------------------------------------------------------------------------------
void OutputHandler::addToQueue(const DataRecordHandle* const dataRecord)
{
   if (dataRecord != nullptr) {
      base::lock( semaphore );
      // const cast away to put into the queue
      queue.put( const_cast<DataRecordHandle*>(static_cast<const DataRecordHandle*>(dataRecord)) );
      base::unlock( semaphore );
   }
}


//------------------------------------------------------------------------------
// Process all data records in the queue.
//------------------------------------------------------------------------------
void OutputHandler::processQueue()
{
   // Get the first record from the queue
   base::lock( semaphore );
   const DataRecordHandle* dataRecord{static_cast<const DataRecordHandle*>(queue.get())};
   base::unlock( semaphore );

   // While we have records ...
   while (dataRecord != nullptr) {
      // process this record,
      processRecord(dataRecord);
      dataRecord->unref();

      // and get the next one from the queue
      base::lock( semaphore );
      dataRecord = static_cast<const DataRecordHandle*>(queue.get());
      base::unlock( semaphore );
   }
}


//------------------------------------------------------------------------------
// processRecordImp() stub
//------------------------------------------------------------------------------
void OutputHandler::processRecordImp(const DataRecordHandle* const)
{
}


//------------------------------------------------------------------------------
// Check the data filters and return true if we should process this type message
//------------------------------------------------------------------------------
bool OutputHandler::isDataTypeEnabled(const DataRecordHandle* const handle) const
{
   unsigned int id{handle->getRecord()->id()};
   return isDataEnabled(id);
}


//------------------------------------------------------------------------------
//  make sure our subcomponents are all of type OutputHandler (or derived)
//------------------------------------------------------------------------------
void OutputHandler::processComponents(
      base::PairStream* const list,
      const std::type_info&,
      base::Pair* const add,
      base::IComponent* const remove
   )
{
   BaseClass::processComponents(list, typeid(OutputHandler), add, remove);
}

}
}
}
