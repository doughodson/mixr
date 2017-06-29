
#ifndef __mixr_models_AvionicsPod_H__
#define __mixr_models_AvionicsPod_H__

#include "mixr/models/system/ExternalStore.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: AvionicsPod
// Description: Generic avionics pod
//
// Factory name: AvionicsPod
//------------------------------------------------------------------------------
class AvionicsPod : public ExternalStore
{
   DECLARE_SUBCLASS(AvionicsPod, ExternalStore)

public:
   AvionicsPod();
};

}
}

#endif
