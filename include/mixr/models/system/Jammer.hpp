
#ifndef __mixr_models_common_Jammer_HPP__
#define __mixr_models_common_Jammer_HPP__

#include "mixr/models/system/RfSensor.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Jammer
// Description: Example Jammer
// Factory name: Jammer
//
// Default R/F sensor type ID is "JAMMER"
//------------------------------------------------------------------------------
class Jammer : public RfSensor
{
    DECLARE_SUBCLASS(Jammer, RfSensor)

public:
    Jammer();

protected:
   void transmit(const double dt) override;
};

}
}

#endif
