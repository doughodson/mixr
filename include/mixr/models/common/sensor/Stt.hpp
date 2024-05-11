
#ifndef __mixr_models_common_Stt_HPP__
#define __mixr_models_common_Stt_HPP__

#include "mixr/models/common/system/Radar.hpp"

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
