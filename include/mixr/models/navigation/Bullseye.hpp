
#ifndef __mixr_models_Bullseye_H__
#define __mixr_models_Bullseye_H__

#include "mixr/models/navigation/Steerpoint.hpp"

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
