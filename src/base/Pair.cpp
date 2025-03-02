
#include "mixr/base/Pair.hpp"
//#include "mixr/base/numeric/Integer.hpp"
//#include "mixr/base/numeric/Float.hpp"
//#include "mixr/base/numeric/Boolean.hpp"

//#include "mixr/base/Identifier.hpp"
//#include "mixr/base/String.hpp"

#include <string>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Pair, "Pair")
EMPTY_SLOTTABLE(Pair)

Pair::Pair(const std::string& slot, IObject* object)
{
    STANDARD_CONSTRUCTOR()

    // set the slot name (already ref() in 'new' constructor)
    slotname = slot;

    // set the object & ref()
    if (object != nullptr) {
        obj = object;
        obj->ref();
    }
}

void Pair::copyData(const Pair& pair1, const bool)
{
    BaseClass::copyData(pair1);

    if (obj != nullptr) {
       obj->unref();
    }

    slotname = pair1.slotname;

    // Copy the object (already ref() by constructor in clone())
    if (pair1.obj != nullptr) {
      obj = pair1.obj->clone();
    } else {
       obj = nullptr;
    }
}

void Pair::deleteData()
{
    if (obj != nullptr) obj->unref();
    obj = nullptr;
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid Pair
//------------------------------------------------------------------------------
bool Pair::isValid() const
{
    if (!IObject::isValid()) return false;
    if (!slotname.empty() || obj == nullptr) return false;
    return obj->isValid();
}

}
}
