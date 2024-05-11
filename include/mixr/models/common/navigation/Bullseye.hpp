
#ifndef __mixr_models_common_Bullseye_HPP__
#define __mixr_models_common_Bullseye_HPP__

#include "mixr/models/common/navigation/Steerpoint.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Bullseye
// Description: Generic Bullseye, used as a reference point
//
// Factory name: Bullseye
//------------------------------------------------------------------------------
class Bullseye : public Steerpoint
{
   DECLARE_SUBCLASS(Bullseye, Steerpoint)

public:
    Bullseye();
};

}
}

#endif
