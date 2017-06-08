
#ifndef __mxrp_models_Stt_H__
#define __mxrp_models_Stt_H__

#include "mxrp/models/system/Radar.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Stt
// Description: Simple, Single-Target-Track Mode Radar
//------------------------------------------------------------------------------
class Stt : public Radar
{
    DECLARE_SUBCLASS(Stt, Radar)

public:
    Stt();

protected:
   virtual void dynamics(const double dt) override;
};

}
}

#endif
