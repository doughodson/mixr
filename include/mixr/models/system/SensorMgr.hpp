
#ifndef __mixr_models_SensorMgr_HPP__
#define __mixr_models_SensorMgr_HPP__

#include "mixr/models/system/IRfSensor.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: SensorMgr
//
// Description: Concrete class for managing a list of R/F sensors
//
// Factory name: ISensorMgr
//------------------------------------------------------------------------------
class SensorMgr : public IRfSensor
{
    DECLARE_SUBCLASS(SensorMgr, IRfSensor)

public:
    SensorMgr();
};

}
}

#endif
