
#ifndef __mixr_models_common_Stt_HPP__
#define __mixr_models_common_Stt_HPP__

#include "mixr/models/system/IRadar.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Stt
// Description: Concrete Simple, Single-Target-Track Mode Radar
//------------------------------------------------------------------------------
class Stt : public IRadar
{
    DECLARE_SUBCLASS(Stt, IRadar)

public:
    Stt();

protected:
   void dynamics(const double dt) override;
};

}
}

#endif
