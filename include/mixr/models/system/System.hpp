
#ifndef __mixr_models_common_System_HPP__
#define __mixr_models_common_System_HPP__

#include "mixr/models/system/ISystem.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: System
//
// Description: A systems class
//
// Factory name: System
//------------------------------------------------------------------------------
class System final: public ISystem
{
   DECLARE_SUBCLASS(System, ISystem)

public:
   System();
};

}
}

#endif
