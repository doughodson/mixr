
#ifndef __mixr_models_common_Gimbal_HPP__
#define __mixr_models_common_Gimbal_HPP__

#include "mixr/models/system/IGimbal.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Gimbal
// Description: Concrete gimbal class
//
// Factory name: Gimbal
//------------------------------------------------------------------------------
class Gimbal final: public IGimbal
{
   DECLARE_SUBCLASS(Gimbal, IGimbal)

public:
   Gimbal();
};

}
}

#endif
