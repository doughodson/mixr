
#ifndef __mixr_models_Stt_H__
#define __mixr_models_Stt_H__

#include "mixr/models/system/Radar.hpp"

namespace mixr {
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
   void dynamics(const double dt) override;
};

}
}

#endif
