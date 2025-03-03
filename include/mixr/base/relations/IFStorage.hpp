
#ifndef __mixr_base_IFStorage_HPP__
#define __mixr_base_IFStorage_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class IFStorage
// Description: Interface for past values of function calls.
//              Use IFunction::storageFactory() to generate pre-ref()'d
//              IFStorage objects.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: IFStorage
// Slots: none
//------------------------------------------------------------------------------
class IFStorage : public IObject
{
   DECLARE_SUBCLASS(IFStorage, IObject)
public:
   IFStorage();
};

}
}

#endif
