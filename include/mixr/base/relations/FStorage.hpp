
#ifndef __mixr_base_FStorage_HPP__
#define __mixr_base_FStorage_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class FStorage
// Description: Generic storage class for past values of function calls.
//              Use Function::storageFactory() to generate pre-ref()'d
//              FStorage objects.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FStorage
// Slots: none
//------------------------------------------------------------------------------
class FStorage : public Object
{
   DECLARE_SUBCLASS(FStorage, Object)
public:
   FStorage();
};

}
}

#endif
