
#ifndef __mixr_base_Stack_HPP__
#define __mixr_base_Stack_HPP__

#include "mixr/base/IList.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Stack
// Description: Concrete class that defines a stack of objects
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Stack
// Slots: none
//------------------------------------------------------------------------------
class Stack final : public IList
{
    DECLARE_SUBCLASS(Stack, IList)

public:
    Stack();

    int operator==(const Stack& list) const;
    int operator!=(const Stack& list) const;

    // pushes the 'object' to the top of the stack.
    void push(IObject* object);
    // pops the object off the top of the stack.  Empty stack will return nullptr.
    IObject* pop();
};

inline void Stack::push(IObject* object)
{
    IList::addHead(object);
}

inline IObject* Stack::pop()
{
    return IList::removeHead();
}

inline int Stack::operator==(const Stack& list) const
{
    const IList* s1 = this;
    const IList* s2 = &list;
    return *s1 == *s2;
}

inline int Stack::operator!=(const Stack& list) const
{
    const IList* s1 = this;
    const IList* s2 = &list;
    return *s1 != *s2;
}

}
}

#endif
