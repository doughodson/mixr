
#ifndef __mixr_models_Jammer_HPP__
#define __mixr_models_Jammer_HPP__

#include "mixr/models/system/IRfSensor.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Jammer
// Description: Concrete Jammer
// Factory name: Jammer
//
// Default R/F sensor type ID is "JAMMER"
//------------------------------------------------------------------------------
class Jammer final: public IRfSensor
{
    DECLARE_SUBCLASS(Jammer, IRfSensor)

public:
    Jammer();

protected:
   void transmit(const double dt) override;
};

}
}

#endif
