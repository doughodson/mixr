
#ifndef __mixr_recorder_IInputHandler_HPP__
#define __mixr_recorder_IInputHandler_HPP__

#include "mixr/simulation/IRecorderComponent.hpp"

namespace mixr {
namespace recorder {
namespace protobuf_v2 {
class DataRecordHandle;

//------------------------------------------------------------------------------
// Class: IInputHandler
// Description: Interface to read and parse data records from an input source
//------------------------------------------------------------------------------
class IInputHandler : public simulation::IRecorderComponent
{
    DECLARE_SUBCLASS(IInputHandler, simulation::IRecorderComponent)

public:
   IInputHandler();

   // Read one data record; returns zero if no record is available
   const DataRecordHandle* readRecord();

protected:
   // Read one record from our data source
   // -- Must be implemented by our derived classes
   virtual const DataRecordHandle* readRecordImp() =0;
};

}
}
}

#endif
