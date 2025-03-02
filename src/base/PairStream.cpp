
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"

#include <cstdlib>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(PairStream, "PairStream")
EMPTY_SLOTTABLE(PairStream)
EMPTY_COPYDATA(PairStream)
EMPTY_DELETEDATA(PairStream)

PairStream::PairStream()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// findByType() -- find a Pair by object type
//------------------------------------------------------------------------------
Pair* PairStream::findByType(const std::type_info& type)
{
    Pair* p{};
    const Item* item{getFirstItem()};
    while (item != nullptr && p == nullptr) {
        const auto pair = const_cast<Pair*>(static_cast<const Pair*>(item->getValue()));
        if (pair->object()->isClassType(type)) p = pair;
        item = item->getNext();
    }
    return p;
}

const Pair* PairStream::findByType(const std::type_info& type) const
{
    const Pair* p{};
    const Item* item{getFirstItem()};
    while (item != nullptr && p == nullptr) {
        const auto pair = static_cast<const Pair*>(item->getValue());
        if (pair->object()->isClassType(type)) p = pair;
        item = item->getNext();
    }
    return p;
}

//------------------------------------------------------------------------------
// findByName() -- find a Pair by name
//------------------------------------------------------------------------------
Pair* PairStream::findByName(const char* const slotname)
{
    Pair* p{};
    if (slotname != nullptr) {
        const Item* item{getFirstItem()};
        while (item != nullptr && p == nullptr) {
            const auto pair = const_cast<Pair*>(static_cast<const Pair*>(item->getValue()));
            if ( pair->slot() == slotname ) p = pair;
            item = item->getNext();
        }
    }
    return p;
}

const Pair* PairStream::findByName(const char* const slotname) const
{
    const Pair* p{};
    if (slotname != nullptr) {
        const Item* item{getFirstItem()};
        while (item != nullptr && p == nullptr) {
            const auto pair = static_cast<const Pair*>(item->getValue());
            if ( pair->slot() == slotname ) p = pair;
            item = item->getNext();
        }
    }
    return p;
}


//------------------------------------------------------------------------------
// findName() -- Finds the name associated with an object.
//------------------------------------------------------------------------------
const std::string PairStream::findName(const IObject* const obj) const
{
    std::string p;
    if (obj != nullptr) {
        const Item* item{getFirstItem()};
        while (item != nullptr && p.empty()) {
            const auto pair = static_cast<const Pair*>(item->getValue());
            if (pair->object() == obj) {
                return pair->slot();
            }
            item = item->getNext();
        }
    }
    return p;
}


//------------------------------------------------------------------------------
// Comparison operators -- use the List operators
//------------------------------------------------------------------------------
bool PairStream::operator==(const PairStream& stream) const
{
    const List* s1{this};
    const List* s2{&stream};
    return *s1 == *s2;
}

bool PairStream::operator!=(const PairStream& stream) const
{
    const List* s1{this};
    const List* s2{&stream};
    return *s1 != *s2;
}

}
}
