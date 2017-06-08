
#ifndef __mxrp_models_Jammer_H__
#define __mxrp_models_Jammer_H__

#include "mxrp/models/system/RfSensor.hpp"

namespace mxrp {
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
   virtual void transmit(const double dt) override;
};

}
}

#endif
