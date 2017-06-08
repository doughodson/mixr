
#ifndef __mxrp_models_AvionicsPod_H__
#define __mxrp_models_AvionicsPod_H__

#include "mxrp/models/system/ExternalStore.hpp"

namespace oe {
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
