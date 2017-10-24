
#include "mixr/models/system/SensorMgr.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SensorMgr, "SensorMgr")
EMPTY_SLOTTABLE(SensorMgr)
EMPTY_COPYDATA(SensorMgr)
EMPTY_DELETEDATA(SensorMgr)

SensorMgr::SensorMgr()
{
    STANDARD_CONSTRUCTOR()
}

}
}
