
#ifndef __mixr_base_Thread_H__
#define __mixr_base_Thread_H__

#include "mixr/base/threads/AbstractThread.hpp"

namespace mixr {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: Thread
// Description: Abstract thread class
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is only called once.
//------------------------------------------------------------------------------
class Thread : public AbstractThread
{
public:
   Thread(Component* const parent);
   Thread(const Thread&) = delete;
   Thread& operator=(const Thread&) = delete;

   // User defined work function
   private: virtual unsigned long userFunc() =0;

private:
   // Thread class function
   unsigned long mainThreadFunc() final;
};

}
}

#endif

