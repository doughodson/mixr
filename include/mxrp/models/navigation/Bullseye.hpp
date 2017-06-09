
#ifndef __mxrp_models_Bullseye_H__
#define __mxrp_models_Bullseye_H__

#include "mxrp/models/navigation/Steerpoint.hpp"

namespace mxrp {
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