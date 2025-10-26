
#ifndef __mixr_recorder_OutputHandler_HPP__
#define __mixr_recorder_OutputHandler_HPP__

#include "mixr/simulation/IRecorder.hpp"
#include "mixr/base/IList.hpp"

namespace mixr {
namespace base { class IList; class IPairStream; }
namespace recorder {
namespace protobuf_v2 {
class DataRecordHandle;

//------------------------------------------------------------------------------
// Class: OutputHandler
// Description: Generic output handler for Protocol Buffer data records
//
// Notes:
//    1) Derived class must implement processRecordImp() to process the
//    data record.
//
//    2) the addToQueue() function will save the record for later processing
//    by the processQueue() function.  This allows a time critical thread to
//    create a data record and queue it for later processing by a background
//    thread, which would call the processQueue() function.
//
//    3) Using the 'components' slot, this OutputHandler can manage as list
//    of subcomponent OutputHandlers.  The prcessRecord() function for each
//    subcomponent OutputHandler is called from our processRecord() function.
//
// Factory name: OutputHandler
//
// Overriding the Component class slot:
//    components     ! Must contain only 'OutputHandler' type objects
//
//------------------------------------------------------------------------------
class OutputHandler : public simulation::IRecorder
{
   DECLARE_SUBCLASS(OutputHandler, simulation::IRecorder)

public:
   OutputHandler();

   // Process the data record
   void processRecord(const DataRecordHandle* const);

   // Add the data record to a queue for later processing
   void addToQueue(const DataRecordHandle* const);

   // Process all data records from the queue
   void processQueue();

protected:
   // Process record implementations by derived classes
   virtual void processRecordImp(const DataRecordHandle* const);

   // Checks the data enabled list and returns true if the record should be processed.
   bool isDataTypeEnabled(const DataRecordHandle* const handle) const;

   void processComponents(
      base::IPairStream* const list,             // Source list of components
      const std::type_info& filter,             // Type filter
      base::Pair* const add = nullptr,          // Optional pair to add
      base::IComponent* const remove = nullptr  // Optional subcomponent to remove
   ) override;

   bool shutdownNotification() override;

private:
   base::IList queue;            // Data Record Queue
   mutable long semaphore {};
};

}
}
}

#endif
