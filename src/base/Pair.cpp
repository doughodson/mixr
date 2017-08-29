
#include "mixr/base/Pair.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/String.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Pair, "Pair")
EMPTY_SLOTTABLE(Pair)

Pair::Pair(const char* slot, Object* object)
{
    STANDARD_CONSTRUCTOR()

    // Set the slot name (already ref() in 'new' constructor)
    slotname = new Identifier(slot);

    // Set the object & ref()
    if (object != nullptr) {
        obj = object;
        obj->ref();
    }
}

void Pair::copyData(const Pair& pair1, const bool)
{
    BaseClass::copyData(pair1);

    // unref() any old data
    if (slotname != nullptr) {
       slotname->unref();
    }

    if (obj != nullptr) {
       obj->unref();
    }

    // Copy slotname (already ref() by constructor in clone())
    if (pair1.slotname != nullptr) {
       slotname = static_cast<Identifier*>(pair1.slotname->clone());
    }
    else {
       slotname = nullptr;
    }

    // Copy the object (already ref() by constructor in clone())
    if (pair1.obj != nullptr) {
      obj = pair1.obj->clone();
    }
    else {
       obj = nullptr;
    }
}

void Pair::deleteData()
{
    if (slotname != nullptr) slotname->unref();
    slotname = nullptr;

    if (obj != nullptr) obj->unref();
    obj = nullptr;
}

//------------------------------------------------------------------------------
// isValid() -- is this a valid Pair
//------------------------------------------------------------------------------
bool Pair::isValid() const
{
    if (!Object::isValid()) return false;
    if (slotname == nullptr || obj == nullptr) return false;
    return slotname->isValid() && obj->isValid();
}

}
}
