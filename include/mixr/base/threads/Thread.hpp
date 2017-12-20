
#ifndef __mixr_base_Thread_H__
#define __mixr_base_Thread_H__

#include "mixr/base/threads/AbstractThread.hpp"

namespace mixr {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: Thread
//
// Description: Abstract thread class that executes a single task. Derived classes
//              need to define the work function (i.e., userFunc()), which is
//              called once.
//------------------------------------------------------------------------------
class Thread : public AbstractThread
{
public:
   Thread(Component* const parent);
   Thread(const Thread&) = delete;
   Thread& operator=(const Thread&) = delete;

   // user defined work function
   private: virtual unsigned long userFunc() =0;

private:
   // thread class function
   unsigned long mainThreadFunc() final;
};

}
}

#endif

