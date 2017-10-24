
#ifndef __mixr_models_SensorMgr_H__
#define __mixr_models_SensorMgr_H__

#include "mixr/models/system/RfSensor.hpp"

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
