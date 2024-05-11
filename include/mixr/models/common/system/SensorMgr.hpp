
#ifndef __mixr_models_common_SensorMgr_HPP__
#define __mixr_models_common_SensorMgr_HPP__

#include "mixr/models/common/system/RfSensor.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: SensorMgr
//
// Description: Generic class for managing a list of R/F sensors
// Factory name: SensorMgr
//------------------------------------------------------------------------------
class SensorMgr : public RfSensor
{
    DECLARE_SUBCLASS(SensorMgr, RfSensor)

public:
    SensorMgr();
};

}
}

#endif
