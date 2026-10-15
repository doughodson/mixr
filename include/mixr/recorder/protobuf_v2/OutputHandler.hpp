
#ifndef __mixr_recorder_OutputHandler_HPP__
#define __mixr_recorder_OutputHandler_HPP__

#include "IOutputHandler.hpp"

namespace mixr {
namespace recorder {
namespace protobuf_v2 {

//------------------------------------------------------------------------------
// Class: OutputHandler
// Description: Concrete for Protocol Buffer data records
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
class OutputHandler : public IOutputHandler
{
   DECLARE_SUBCLASS(IOutputHandler, IOutputHandler)

public:
   OutputHandler();

}
}
}

#endif
