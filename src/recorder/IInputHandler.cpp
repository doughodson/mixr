
#include "mixr/recorder/IInputHandler.hpp"
#include "mixr/recorder/DataRecordHandle.hpp"
#include "mixr/recorder/protobuf/DataRecord.pb.h"

namespace mixr {
namespace recorder {

IMPLEMENT_ABSTRACT_SUBCLASS(IInputHandler, "IInputHandler")
EMPTY_SLOTTABLE(IInputHandler)
EMPTY_CONSTRUCTOR(IInputHandler)
EMPTY_COPYDATA(IInputHandler)
EMPTY_DELETEDATA(IInputHandler)

//------------------------------------------------------------------------------
// Read one data record; returns zero if no record is available
//------------------------------------------------------------------------------
const DataRecordHandle* IInputHandler::readRecord()
{
   const DataRecordHandle* p{};
   bool finished{};

   while (!finished) {

      // Get a record from our derived class
      p = readRecordImp();

      // Did we get one?
      if (p != nullptr) {
         // Yes -- now check to see if this data record is enabled
         unsigned int id{p->getRecord()->id()};
         bool enabled{isDataEnabled(id)};

         if (enabled) finished = true;    // If it is then we're done
         else { p->unref(); p = nullptr; }      // If itis disabled then we'll try again
      }

      // When we didn't receive a record then we're done for now and they
      // can try again later
      else finished = true;

   }

   return p;
}

}
}
