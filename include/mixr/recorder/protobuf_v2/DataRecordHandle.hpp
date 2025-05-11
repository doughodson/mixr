
#ifndef __mixr_recorder_DataRecordHandle_HPP__
#define __mixr_recorder_DataRecordHandle_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace recorder {
namespace protobuf_v2 {
// Main (protocol buffer) data record
namespace proto { class DataRecord; }

//------------------------------------------------------------------------------
// Class: DataRecordHandle
// Description: Handle to a DataRecord, so we can ref count it.
//
// Notes:
//
//    1) This handle will 'own' the DataRecord ...
//
//    2) When this handle is destroyed, the DataRecord will be deleted.
//
//    3) Using the assignment operator ( e.g., handle1 = handle2; ), the contents
//       of handle2's DataRecord will be copied into handle1's DataRecord.
//
//------------------------------------------------------------------------------
class DataRecordHandle : public base::IObject
{
    DECLARE_SUBCLASS(DataRecordHandle, base::IObject)

public:
   DataRecordHandle(proto::DataRecord* const record);

   const proto::DataRecord* getRecord() const;

protected:
   DataRecordHandle();  // Default Constructor

private:
   proto::DataRecord* record {};
};

inline const proto::DataRecord* DataRecordHandle::getRecord() const { return record; }

}
}
}

#endif
