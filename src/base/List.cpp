
#include "mixr/base/List.hpp"

#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Pair.hpp"

#include <cstdlib>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(List, "List")
EMPTY_SLOTTABLE(List)

List::List()
{
    STANDARD_CONSTRUCTOR()
}

List::List(const double values[], const std::size_t nv)
{
    STANDARD_CONSTRUCTOR()

    // Create Float's for each value and add to the list.
    for (std::size_t i = 0; i < nv; i++) {
        const auto p{new Float(values[i])};
        put(p);
        p->unref(); // ref() by put(), so we can unref().
    }
}

List::List(const int values[], const std::size_t nv)
{
    STANDARD_CONSTRUCTOR()

    // Create Integer's for each value and add to the list.
    for (std::size_t i = 0; i < nv; i++) {
        const auto p{new Integer(values[i])};
        put(p);
        p->unref(); // ref() by put(), so we can unref().
    }
}

void List::copyData(const List& org, const bool)
{
    BaseClass::copyData(org);

    // Clear the old list (if any)
    clear();

    // Copy the new list
    const Item* d{org.getFirstItem()};
    for ( ; d != nullptr; d = d->getNext() ) {
        Object* p{d->getValue()->clone()};
        if (p != nullptr) {
            addTail(p);
            p->unref();     // p is ref() by addTail(), so we can unref();
        }
    }
}

void List::deleteData()
{
    clear();
}

// is this a valid List
bool List::isValid() const
{
    bool ok{Object::isValid()};
    for (const Item* d = headP; ok && d != nullptr; d = d->getNext() ) {
        const Object* obj{d->getValue()};
        if (obj != nullptr) {
            if (!obj->isValid()) ok = false;
        } else {
           ok = false;
        }
    }
    return ok;
}

// clear out (or empty) the list
void List::clear()
{
    // Empty out the list ...
    while (!isEmpty()) {
        Object* p{removeHead()}; // First remove them
        if (p != nullptr) {
            p->unref();     // and unref() them
        }
    }
    headP = nullptr;
    tailP = nullptr;
    num = 0;
}


// find object on the list
std::size_t List::getIndex(const Object* const x) const
{
    const Item* p{};
    std::size_t idx{};
    for (const Item* d = headP; d != nullptr; d = d->getNext() ) {
        idx++;
        if ( x == d->getValue() )  {
            p = d;
            break;
        }
    }
    return ((p != nullptr) ? idx : 0);
}

// adds object to the head of the list.
void List::addHead(Object* const obj)
{
    if (obj == nullptr) return;
    const auto d = new Item;
    d->value = obj;
    obj->ref();
    addHead(d);
}

// adds object to the tail of the list.
void List::addTail(Object* const obj)
{
    if (obj == nullptr) return;
    const auto d = new Item;
    d->value = obj;
    obj->ref();
    addTail(d);
}

// removes object from the list.
bool List::remove(const Object* const obj)
{
    bool ok{};
    if (obj != nullptr) {
        Item* d{headP};
        for ( ; d != nullptr; d = d->getNext() ) {
            if ( obj == d->getValue() )  break;
        }
        if (d != nullptr) {
           remove(d);
           ok = true;
        }
    }
    if (ok) obj->unref();
    return ok;
}

//------------------------------------------------------------------------------
// int getNumberList(float* values, int max)
// int getNumberList(int* values, int max)
//      This routine is a method to retrieve an array of Numbers (or
//      Pairs that contain Numbers) from a list.  A maximum
//      of 'max' items from the list are placed in the array 'values'.
//      A zero is placed in 'values' when the item is not a Number.
//      The number of values placed in 'values' is returned by
//      getNumberList().
//------------------------------------------------------------------------------
std::size_t List::getNumberList(double values[], const std::size_t max) const
{
    std::size_t n{};
    for (const Item* p = getFirstItem(); p != nullptr && n < max; p = p->getNext() ) {
        const Object* p1{p->getValue()};
        const auto gp = dynamic_cast<const Pair*>(p1);
        if (gp != nullptr) {
            // when the item is a Pair, use the object it contains.
            p1 = gp->object();
        }
        const auto pp = dynamic_cast<const INumber*>(p1);
        if (pp != nullptr) {
            // when we have a number
            values[n++] = pp->asDouble();
        } else {
            // when we do not have a number
            values[n++] = 0.0;
        }
    }
    return n;
}

std::size_t List::getNumberList(float values[], const std::size_t max) const
{
    std::size_t n{};
    for (const Item* p = getFirstItem(); p != nullptr && n < max; p = p->getNext() ) {
        const Object* p1{p->getValue()};
        const auto gp = dynamic_cast<const Pair*>(p1);
        if (gp != nullptr) {
            // when the item is a Pair, use the object it contains.
            p1 = gp->object();
        }
        const auto pp = dynamic_cast<const INumber*>(p1);
        if (pp != nullptr) {
            // when we have a number
            values[n++] = static_cast<float>(pp->asDouble());
        } else {
            // when we do not have a number
            values[n++] = 0.0f;
        }
    }
    return n;
}


std::size_t List::getNumberList(int values[], const std::size_t max) const
{
    std::size_t n{};
    for (const Item* p = getFirstItem(); p != nullptr && n < max; p = p->getNext() ) {
        const Object* p1{p->getValue()};
        const auto gp = dynamic_cast<const Pair*>(p1);
        if (gp != nullptr) {
            // when the item is a Pair, use the object it contains.
            p1 = gp->object();
        }
        const auto pp = dynamic_cast<const INumber*>(p1);
        if (pp != nullptr) {
            // when we have a number
            //values[n++] = pp->getInt(); - revised by DDH
            values[n++] = static_cast<int>(pp->asDouble());
        } else {
            // when we do not have a number
            values[n++] = 0;
        }
    }
    return n;
}

// removes the item at the head of the list. (Ownership is passed to caller -- does not unref())
Object* List::removeHead()
{
    Object* p{};
    if (headP != nullptr) {
        Item* d{headP};
        headP = headP->next;
        p = d->getValue();
        num--;
        if (headP != nullptr) headP->previous = nullptr;
        else tailP = nullptr;
        delete d;
    }
    return p;
}

// removes the item at the tail of the list. (Ownership passed to caller -- does not unref())
Object* List::removeTail()
{
    Object* p{};
    if (tailP != nullptr) {
        Item *d{tailP};
        tailP = tailP->previous;
        p = d->getValue();
        num--;
        if (tailP != nullptr) tailP->next = nullptr;
        else headP = nullptr;
        delete d;
    }
    return p;
}

// insert a new item before 'refItem'.  If 'refItem' is
// nullptr, the new item is added to the tail of the list.
bool List::insert(List::Item* newItem, List::Item* refItem)
{
    bool ok{true};
    if (refItem != nullptr) {
        if (refItem == headP) {
            addHead(newItem);
        } else {
            newItem->previous = refItem->previous;
            refItem->previous = newItem;
            newItem->previous->next = newItem;
            newItem->next = refItem;
            num++;
        }
    } else {
        addTail(newItem);
    }
    return ok;
}

// removes the Item from the list. (Ownership passed to caller -- does not unref())
Object* List::remove(List::Item* item)
{
    Object* value{};
    if (headP == item)
        value = removeHead();
    else if (tailP == item)
        value = removeTail();
    else if (item != nullptr) {
        value = item->getValue();
        num--;
        Item* p{item->getPrevious()};
        Item* n{item->getNext()};
        n->previous = p;
        p->next     = n;
        delete item;
    }
    return value;
}

// adds the Item to the head of the list.
void List::addHead(List::Item* item)
{
    item->previous = nullptr;
    item->next = headP;
    if (headP != nullptr) headP->previous = item;
    headP = item;
    if (tailP == nullptr) tailP = item;
    num++;
}

// adds the Item to the tail of the list.
void List::addTail(List::Item* item)
{
    item->next = nullptr;
    item->previous = tailP;
    if (tailP != nullptr) tailP->next = item;
    tailP = item;
    if (headP == nullptr) headP = item;
    num++;
}

// returns true if two lists are equal.
bool List::operator==(const List& l) const
{
   if (entries() != l.entries()) return false;

   const Item* tt{getFirstItem()};
   const Item* ll{l.getFirstItem()};
   while (tt != nullptr) {
      if (tt->getValue() != ll->getValue()) return false;
      tt = tt->getNext();
      ll = ll->getNext();
   }
   return true;
}

// returns true if two lists are NOT equal.
bool List::operator!=(const List& l) const
{
    return !(*this == l);
}

// returns the object at the n'th position; w/o next pointer
const Object* List::getPosition1(const std::size_t n) const
{
    if (n < 1 || n > num) return nullptr;
    std::size_t i{1};
    const Item* p{getFirstItem()};
    while (i < n && p != nullptr) {
        p = p->getNext();
        i++;
    }
    if (p != nullptr)
        return p->getValue();
    else
        return nullptr;
}

}
}
