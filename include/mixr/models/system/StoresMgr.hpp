
#ifndef __mixr_models_StoresMgr_HPP__
#define __mixr_models_StoresMgr_HPP__

#include "mixr/models/system/IStoresMgr.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: StoresMgr
// Description: Concrete stores manager
// Factory name: StoresMgr
//------------------------------------------------------------------------------
class StoresMgr final: public IStoresMgr
{
    DECLARE_SUBCLASS(StoresMgr, IStoresMgr)

public:
    StoresMgr();
};

}
}

#endif
