
#include "mixr/base/IPairStream.hpp"
#include "mixr/base/Pair.hpp"

#include <cstdlib>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(IPairStream, "IPairStream")
EMPTY_SLOTTABLE(IPairStream)
EMPTY_COPYDATA(IPairStream)
EMPTY_DELETEDATA(IPairStream)

IPairStream::IPairStream()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// findByType() -- find a Pair by object type
//------------------------------------------------------------------------------
Pair* IPairStream::findByType(const std::type_info& type)
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

const Pair* IPairStream::findByType(const std::type_info& type) const
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
Pair* IPairStream::findByName(const char* const slotname)
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

const Pair* IPairStream::findByName(const char* const slotname) const
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
const std::string IPairStream::findName(const IObject* const obj) const
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
bool IPairStream::operator==(const IPairStream& stream) const
{
    const IList* s1{this};
    const IList* s2{&stream};
    return *s1 == *s2;
}

bool IPairStream::operator!=(const IPairStream& stream) const
{
    const IList* s1{this};
    const IList* s2{&stream};
    return *s1 != *s2;
}

}
}
