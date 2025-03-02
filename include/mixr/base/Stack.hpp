
#ifndef __mixr_base_Stack_HPP__
#define __mixr_base_Stack_HPP__

#include "mixr/base/List.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Stack
// Description: Stacks of objects
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Stack
// Slots: none
//------------------------------------------------------------------------------
class Stack : public List
{
    DECLARE_SUBCLASS(Stack, List)

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
    List::addHead(object);
}

inline IObject* Stack::pop()
{
    return List::removeHead();
}

inline int Stack::operator==(const Stack& list) const
{
    const List* s1 = this;
    const List* s2 = &list;
    return *s1 == *s2;
}

inline int Stack::operator!=(const Stack& list) const
{
    const List* s1 = this;
    const List* s2 = &list;
    return *s1 != *s2;
}

}
}

#endif
