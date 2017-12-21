
#ifndef __mixr_base_OneShotThread_H__
#define __mixr_base_OneShotThread_H__

#include "mixr/base/threads/AbstractThread.hpp"

namespace mixr {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: OneShotThread
//
// Description: Abstract thread class that executes a single task. Derived classes
//              need to define the work function (i.e., userFunc()), which is
//              called once. (Note: This class was named 'One Shot' to avoid
//              confusion with a new class to be introduced called 'Thread' that
//              will use std::thread functionality.)
//------------------------------------------------------------------------------
class OneShotThread : public AbstractThread
{
public:
   OneShotThread(Component* const parent);
   OneShotThread(const OneShotThread&) = delete;
   OneShotThread& operator=(const OneShotThread&) = delete;

private:
   unsigned long mainThreadFunc() final;

   // user defined work function
   virtual unsigned long userFunc() =0;
};

}
}

#endif

