
#include "mixr/recorder/protobuf_v2/DataRecordHandle.hpp"
#include "mixr/recorder/protobuf_v2/proto/DataRecord.pb.h"

namespace mixr {
namespace recorder {
namespace protobuf_v2 {

IMPLEMENT_SUBCLASS(DataRecordHandle,"DataRecordHandle")
EMPTY_SLOTTABLE(DataRecordHandle)

DataRecordHandle::DataRecordHandle()
{
   STANDARD_CONSTRUCTOR()
}

DataRecordHandle::DataRecordHandle(proto::DataRecord* const r) : record(r)
{
   STANDARD_CONSTRUCTOR()
}

void DataRecordHandle::copyData(const DataRecordHandle& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) record = new proto::DataRecord();

   // Copy the record
   *record = *org.record;
}

void DataRecordHandle::deleteData()
{
   if (record != nullptr) { delete record;  record = nullptr; }
}

}
}
}
