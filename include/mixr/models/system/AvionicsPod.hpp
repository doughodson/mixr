
#ifndef __mixr_models_common_AvionicsPod_HPP__
#define __mixr_models_common_AvionicsPod_HPP__

#include "mixr/models/system/IExternalStore.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: AvionicsPod
// Description: Concrete avionics pod
//
// Factory name: AvionicsPod
//------------------------------------------------------------------------------
class AvionicsPod final: public IExternalStore
{
   DECLARE_SUBCLASS(AvionicsPod, IExternalStore)

public:
   AvionicsPod();
};

}
}

#endif
